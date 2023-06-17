//
// Created by volt on 2023-06-17.
//

#ifndef AXXEGRO_FREQPERIOD_HPP
#define AXXEGRO_FREQPERIOD_HPP

#include <chrono>

namespace al {

	struct Freq {
		Freq() = default;
		Freq(const Freq&) = default;
		Freq& operator=(const Freq&) = default;
		Freq(Freq&&) = default;
		Freq& operator=(Freq&&) = default;

		static Freq Hz(double val) { return Freq {val}; }
		static Freq kHz(double val) { return Freq {val * 1000.0}; }

		[[nodiscard]] double getFreqHz() const {
			return valueHz;
		}
		[[nodiscard]] double getPeriodSecs() const {
			return 1.0 / valueHz;
		}
	private:
		explicit Freq(double valueHz) : valueHz(valueHz) {}
		double valueHz;
	};

	inline Freq Hz(double val) {
		return Freq::Hz(val);
	}

	inline Freq kHz(double val) {
		return Freq::kHz(val);
	}

	namespace FreqLiterals {

		Freq operator""_Hz(long double val) {
			return Freq::Hz(static_cast<double>(val));
		}
		Freq operator""_Hz(unsigned long long val) {
			return Freq::Hz(static_cast<double>(val));
		}

		Freq operator""_kHz(long double val) {
			return Freq::kHz(static_cast<double>(val) * 1000.0);
		}
		Freq operator""_kHz(unsigned long long val) {
			return Freq::Hz(static_cast<double>(val) * 1000.0);
		}
	}


	struct Period {
		Period() = default;
		Period(const Period&) = default;
		Period& operator=(const Period&) = default;
		Period(Period&&) = default;
		Period& operator=(Period&&) = default;

		template<typename DurT>
		Period(DurT duration) {
			valueSecs = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
		}

		[[nodiscard]] double getFreqHz() const {
			return 1.0 / valueSecs;
		}
		[[nodiscard]] double getPeriodSecs() const {
			return valueSecs;
		}
	private:
		double valueSecs;
	};

	template<typename T>
	concept FreqOrPeriod = requires(T fp) {
		{fp.getPeriodSecs()} -> std::convertible_to<double>;
		{fp.getFreqHz()} -> std::convertible_to<double>;
	};
}

#endif //AXXEGRO_FREQPERIOD_HPP
