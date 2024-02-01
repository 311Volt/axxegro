/**
 * @file
 * basically a stupid hand-rolled version of std::chrono::duration
 * because chrono does not have the concept of frequency
 * and takes too long to compile
 */

#ifndef AXXEGRO_FREQPERIOD_HPP
#define AXXEGRO_FREQPERIOD_HPP

#include <ratio>

namespace al {

	namespace detail {
		template<typename T>
		struct ValueWrapper {
			constexpr explicit ValueWrapper(T value): value(value) {}
			T value;
		};

		template<typename T>
		concept RatioType = requires {
			{T::num} -> std::convertible_to<std::intmax_t>;
			{T::den} -> std::convertible_to<std::intmax_t>;
			{std::ratio_equal_v<T, T>} -> std::convertible_to<bool>;
		};

		template<RatioType T>
		using InverseRatio = std::ratio<T::den, T::num>;

		template<RatioType T>
		constexpr double RatioAsDouble() {
			return double(T::num) / double(T::den);
		}
	}


	template<typename T>
	concept FreqType = requires(T freq) {
		{freq.getHz()} -> std::convertible_to<double>;
	};
	template<typename T>
	concept PeriodType = requires(T period) {
		{period.getSeconds()} -> std::convertible_to<double>;
	};

	template<typename T>
	concept FreqOrPeriod = FreqType<T> || PeriodType<T>;

	template<detail::RatioType RatioT>
	struct Freq: public detail::ValueWrapper<double> {
		using ValueWrapper::ValueWrapper;
		using RatioType = RatioT::type;


		[[nodiscard]] constexpr double getHz() const {
			return value * detail::RatioAsDouble<RatioType>();
		}

		constexpr operator Freq<std::ratio<1>>() const {
			return Freq<std::ratio<1>>(getHz());
		}

		template<PeriodType PeriodT>
		explicit constexpr operator PeriodT() const {
			using ConvRatio = std::ratio_multiply<
				RatioType,
				detail::InverseRatio<typename PeriodT::RatioType>
			>;

			return PeriodT((1.0 / value) * detail::RatioAsDouble<ConvRatio>());
		}
	};

	template<detail::RatioType RatioT>
	struct Period: public detail::ValueWrapper<double> {
		using ValueWrapper::ValueWrapper;
		using RatioType = RatioT::type;

		[[nodiscard]] constexpr double getSeconds() const {
			return value * detail::RatioAsDouble<RatioType>();
		}

		constexpr operator Period<std::ratio<1>>() const {
			return Period<std::ratio<1>>(getSeconds());
		}

		template<FreqType FreqT>
		explicit constexpr operator FreqT() const {
			using ConvRatio = std::ratio_multiply<
				detail::InverseRatio<typename FreqT::RatioType>,
				detail::InverseRatio<RatioType>
			>;

			return FreqT((1.0 / value) * detail::RatioAsDouble<ConvRatio>());
		}
	};


	using Hz = Freq<std::ratio<1>>;
	using kHz = Freq<std::kilo>;

	using Seconds = Period<std::ratio<1>>;
	using Millis = Period<std::milli>;

	inline namespace Literals {
		inline namespace TimeLiterals {
			inline namespace FreqLiterals {
				inline auto operator""_Hz(long double val) {return Hz(static_cast<double>(val));}
				inline auto operator""_Hz(unsigned long long val) {return Hz(static_cast<double>(val));}
				inline auto operator""_kHz(long double val) {return kHz(static_cast<double>(val));}
				inline auto operator""_kHz(unsigned long long val) {return kHz(static_cast<double>(val));}
			}
			inline namespace PeriodLiterals {
				inline auto operator""_s(long double val) {return Seconds(static_cast<double>(val));}
				inline auto operator""_s(unsigned long long val) {return Seconds(static_cast<double>(val));}
				inline auto operator""_ms(long double val) {return Millis(static_cast<double>(val));}
				inline auto operator""_ms(unsigned long long val) {return Millis(static_cast<double>(val));}
			}
		}

	}

}

#endif //AXXEGRO_FREQPERIOD_HPP
