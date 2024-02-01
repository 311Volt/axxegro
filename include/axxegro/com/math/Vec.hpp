
#ifndef AXXEGRO_VEC_HPP
#define AXXEGRO_VEC_HPP

#include <array>
#include <concepts>
#include <functional>
#include <cmath>
#include <format>
#include <utility>
#include <stdint.h>


namespace al {

	namespace detail {

		/*
		 * Provides named element data members (x, y, z, w) for smaller vectors.
		 */
		template<typename T, int N>
		struct BaseNamedCoord {};

		struct Empty {};

		template<typename T, int N>
		struct BaseNamedCoords {
			static_assert(N > 0 && N <= 4);
		};


		template<typename T>
		struct BaseNamedCoords<T, 1> {
			using ElementType = T;
			static constexpr int NumDimensions = 1;

			T x;

			static constexpr bool CanAliasWithArray =
				   offsetof(BaseNamedCoords, x) == 0;
		};

		template<typename T>
		struct BaseNamedCoords<T, 2> {
			using ElementType = T;
			static constexpr int NumDimensions = 2;

			T x, y;

			static constexpr bool CanAliasWithArray =
				   offsetof(BaseNamedCoords, x) == 0*sizeof(T)
				&& offsetof(BaseNamedCoords, y) == 1*sizeof(T);
		};

		template<typename T>
		struct BaseNamedCoords<T, 3> {
			using ElementType = T;
			static constexpr int NumDimensions = 3;

			T x, y, z;

			static constexpr bool CanAliasWithArray =
				   offsetof(BaseNamedCoords, x) == 0*sizeof(T)
				&& offsetof(BaseNamedCoords, y) == 1*sizeof(T)
				&& offsetof(BaseNamedCoords, z) == 2*sizeof(T);
		};

		template<typename T>
		struct BaseNamedCoords<T, 4> {
			using ElementType = T;
			static constexpr int NumDimensions = 4;

			T x, y, z, w;

			static constexpr bool CanAliasWithArray =
				   offsetof(BaseNamedCoords, x) == 0*sizeof(T)
				&& offsetof(BaseNamedCoords, y) == 1*sizeof(T)
				&& offsetof(BaseNamedCoords, z) == 2*sizeof(T)
				&& offsetof(BaseNamedCoords, w) == 3*sizeof(T);
		};

		template<typename CoordT>
			requires std::remove_cvref_t<CoordT>::CanAliasWithArray
		constexpr auto GetNamedCoordsBegin(CoordT&& coords) {
			return &coords.x;
		}

		template<typename CoordT>
			requires std::remove_cvref_t<CoordT>::CanAliasWithArray
		constexpr auto GetNamedCoordsEnd(CoordT&& coords) {
			return (&coords.x) + std::remove_cvref_t<CoordT>::NumElements;
		}


		template<typename CoordT>
		constexpr auto&& GetNamedCoordAt(CoordT&& coords, int n) {
			using RealCoordT = std::remove_cvref_t<CoordT>;
			constexpr int NDims = RealCoordT::NumDimensions;

#ifndef AXXEGRO_PEDANTIC_STRICT_ALIASING

			if(not std::is_constant_evaluated()) {
				if constexpr (std::remove_cvref_t<CoordT>::CanAliasWithArray) {
					auto begin = &coords.x;
					return reinterpret_cast<decltype(begin)>(begin)[n];
				}
			}

#endif

			/* an out-of-range value of N is intentionally UB, with the hope that some future
			 * compiler will be able to turn these switch statements into ((ValueType*)coords)[n]
			 *
			 * as i'm writing this, none of the 3 major compilers can do this, hence the above */

			if constexpr (NDims == 1) {
				switch (n) {
					case 0: return coords.x;
				}
			} else if constexpr (NDims == 2) {
				switch (n) {
					case 0: return coords.x;
					case 1: return coords.y;
				}
			} else if constexpr (NDims == 3) {
				switch (n) {
					case 0: return coords.x;
					case 1: return coords.y;
					case 2: return coords.z;
				}
			} else if constexpr (NDims == 4) {
				switch (n) {
					case 0: return coords.x;
					case 1: return coords.y;
					case 2: return coords.z;
					case 3: return coords.w;
				}
			}

			if(std::is_constant_evaluated()) {
				throw "constexpr error: out of bounds coords access";
			}

			//suppress -Wreturn-type
			return *static_cast<decltype(&coords.x)>(nullptr);
		}

		/*
		 * Provides the array data member for larger vectors.
		 */
		template<typename T, int N>
		struct BaseArrCoords {
			std::array<T, N> coords;
			static constexpr int NumDimensions = N;
			using ElementType = T;
			using BAC_ID = std::true_type;
		};

		template<typename CoordT>
		constexpr auto GetArrCoordsBegin(CoordT& coords) {
			return coords.coords.begin();
		}

		template<typename CoordT>
		constexpr auto GetArrCoordsEnd(CoordT&& coords) {
			return coords.coords.end();
		}

		template<typename CoordT>
		constexpr auto&& GetArrCoordAt(CoordT&& coords, int n) {
			if(std::is_constant_evaluated() && (n<0 || n>coords.coords.size())) {
				throw "constexpr error: out of bounds coords access";
			}
			return coords.coords[n];
		}


		template<typename T>
		concept NamedCoordsType = requires(T coords){coords.x;};

		template<typename T>
		concept ArrCoordsType = requires(T coords){coords.coords;};

		template<NamedCoordsType CoordT>
		constexpr auto GetCoordsBegin(CoordT&& coords) {
			return GetNamedCoordsBegin(coords);
		}

		template<ArrCoordsType CoordT>
		constexpr auto GetCoordsBegin(CoordT&& coords) {
			return GetArrCoordsBegin(coords);
		}

		template<NamedCoordsType CoordT>
		constexpr auto GetCoordsEnd(CoordT&& coords) {
			return GetNamedCoordsEnd(coords);
		}

		template<ArrCoordsType CoordT>
		constexpr auto GetCoordsEnd(CoordT&& coords) {
			return GetArrCoordsEnd(coords);
		}

		template<NamedCoordsType CoordT>
		constexpr auto&& GetCoordAt(CoordT&& coords, int n) {
			return GetNamedCoordAt(coords, n);
		}

		template<ArrCoordsType CoordT>
		constexpr auto&& GetCoordAt(CoordT&& coords, int n) {
			return GetArrCoordAt(coords, n);
		}

		
		inline constexpr bool ShouldImplementWithArr(int numElements) {
			return numElements > 4;
		}

		template<typename T, int N>
		using DefaultVecBase = std::conditional_t<
			ShouldImplementWithArr(N),
			detail::BaseArrCoords<T,N>,
			detail::BaseNamedCoords<T,N>
		>;
		
		template<typename Derived>
		struct AddVec3CrossProduct {
			[[nodiscard]] constexpr Derived cross(const Derived& other) const {
				return static_cast<const Derived*>(this)->internalCross(other);
			}
		};
	}
		
	template<typename T, int N>
	struct Vec:
		public detail::DefaultVecBase<T, N>,
		public std::conditional_t<N==3, detail::AddVec3CrossProduct<Vec<T, N>>, detail::Empty>
	{
		friend struct detail::AddVec3CrossProduct<Vec>;


		static_assert(N > 0);


		using ValueType = T;
		using ElementType = T;
		static constexpr int NumElements = N;
		static constexpr int IsContiguous = (sizeof(Vec) == sizeof(T)*NumElements);

		static constexpr bool isIndexValid(int i) {
			return i>0 && i<NumElements;
		}

		constexpr ValueType& operator[](int i) {
			return detail::GetCoordAt(*this, i);
		}

		constexpr const ValueType& operator[](int i) const {
			return detail::GetCoordAt(*this, i);
		}

		constexpr ValueType& at(int i) {
			if(not isIndexValid(i)) {
				throw std::out_of_range("Vec: invalid subscript");
			}
			return detail::GetCoordAt(*this, i);
		}

		constexpr const ValueType& at(int i) const {
			if(not isIndexValid(i)) {
				throw std::out_of_range("Vec: invalid subscript");
			}
			return detail::GetCoordAt(*this, i);
		}

		[[nodiscard]] constexpr int size() const {
			return NumElements;
		}

		ValueType* begin() {
			return detail::GetCoordsBegin(*this);
		}

		const ValueType* begin() const {
			return detail::GetCoordsBegin(*this);
		}

		ValueType* end() {
			return detail::GetCoordsEnd(*this);
		}

		const ValueType* end() const {
			return detail::GetCoordsEnd(*this);
		}

		ValueType* data() {
			return detail::GetCoordsBegin(*this);
		}

		const ValueType* data() const {
			return detail::GetCoordsBegin(*this);
		}


		template<std::invocable<ValueType&> Func>
		constexpr Vec& forEach(Func&& fn) {
			for(int i=0; i<NumElements; i++) {
				fn((*this)[i]);
			}
			return *this;
		}

		template<std::invocable<const ValueType&> Func>
		constexpr const Vec& forEach(Func&& fn) const {
			for(int i=0; i<NumElements; i++) {
				fn((*this)[i]);
			}
			return *this;
		}

		template<std::invocable<ValueType&, int> Func>
		constexpr Vec& forEachIndexed(Func&& fn) {
			for(int i=0; i<NumElements; i++) {
				fn((*this)[i], i);
			}
			return *this;
		}

		template<std::invocable<const ValueType&, int> Func>
		constexpr const Vec& forEachIndexed(Func&& fn) const {
			for(int i=0; i<NumElements; i++) {
				fn((*this)[i], i);
			}
			return *this;
		}

		template<std::invocable<ValueType&, const ValueType&> Func>
		constexpr Vec& forEachPair(const Vec& other, Func&& fn) {
			for(int i=0; i<NumElements; i++) {
				fn((*this)[i], other[i]);
			}
			return *this;
		}

		template<std::invocable<const ValueType&> Func>
		constexpr Vec map(Func&& fn) const {
			Vec ret;
			ret.forEach([&](ValueType& v) {
				v = fn(v);
			});
			return ret;
		}

		template<std::invocable<ValueType&, const ValueType&> Op>
		[[nodiscard]] constexpr ValueType accumulate(Op op, ValueType acc = 0) const {
			forEach([&](const ValueType& v) {
				op(acc, v);
			});
			return acc;
		}


		constexpr void fill(ValueType v) {
			forEach([v](ValueType& dv){dv = v;});
		}

		template<std::convertible_to<ValueType>... TValues>
		constexpr void setValues(TValues... values) {
			[&]<std::size_t... Idxs>([[maybe_unused]] std::index_sequence<Idxs...> seq){
				(((*this)[Idxs] = values), ...);
			}(std::make_index_sequence<std::min<int>(sizeof...(TValues), NumElements)>{});
		}


		constexpr Vec& operator+=(const Vec& rhs) {
			forEachPair(rhs, [](ValueType& dst, const ValueType& src){dst += src;});
			return *this;
		}
		constexpr Vec& operator-=(const Vec& rhs) {
			forEachPair(rhs, [](ValueType& dst, const ValueType& src){dst -= src;});
			return *this;
		}

		constexpr Vec& operator*=(const ValueType& rhs) {
			forEach([rhs](ValueType& dst){dst *= rhs;});
			return *this;
		}
		constexpr Vec& operator/=(const ValueType& rhs) {
			forEach([rhs](ValueType& dst){dst /= rhs;});
			return *this;
		}

		constexpr Vec operator+() const {
			return *this;
		}
		constexpr Vec operator-() const {
			Vec ret = *this;
			ret.forEach([](ValueType& v){v = -v;});
			return ret;
		}

		constexpr Vec operator+(const Vec& rhs) const {
			Vec ret=*this; ret+=rhs; return ret;
		}
		constexpr Vec operator-(const Vec& rhs) const {
			Vec ret=*this; ret-=rhs; return ret;
		}
		constexpr Vec operator*(ValueType rhs) const {
			Vec ret=*this; ret*=rhs; return ret;
		}
		constexpr Vec operator/(ValueType rhs) const {
			Vec ret=*this; ret/=rhs; return ret;
		}

		constexpr bool operator==(const Vec& rhs) const {
			for(int i=0; i<NumElements; i++) {
				if((*this)[i] != rhs[i]) {
					return false;
				}
			}
			return true;
		}

		constexpr bool operator!=(const Vec& rhs) const {
			return !(*this == rhs);
		}

		[[nodiscard]] constexpr bool almostEqual(const Vec& rhs, float maxSqDist = 1e-12) const {
			return (rhs - (*this)).sqLength() < maxSqDist;
		}

		[[nodiscard]] constexpr Vec hadamard(const Vec& other) const {
			Vec ret = *this;
			ret.forEachPair(other, [](ValueType& dst, const ValueType& src) {
				dst *= src;
			});
			return ret;
		}

		[[nodiscard]] constexpr ValueType sqLength() const {
			return this->hadamard(*this).accumulate([](ValueType& acc, const ValueType& val) {
				acc += val;
			});
		}

		[[nodiscard]] constexpr double length() const {
			return std::sqrt(sqLength());
		}

		[[nodiscard]] constexpr Vec normalizedOr(Vec val, float tol = 1e-8) const {
			auto len = this->length();
			if(len >= tol) {
				return (*this) / len;
			} else {
				return val;
			}
		}

		[[nodiscard]] constexpr Vec normalized(float tol = 1e-8) const {
			return normalizedOr({}, tol);
		}

		[[nodiscard]] constexpr ValueType dot(const Vec& rhs) const {
			return (this->hadamard(rhs)).accumulate([](ValueType& acc, const ValueType& val) {
				acc += val;
			});
		}

		[[nodiscard]] constexpr Vec rotated(int r) const {
			const int rVal = ((r % N) + N) % N;
			Vec ret;

			for(int i=0; i<NumElements; i++) {
				ret[(i + rVal) % NumElements] = (*this)[i];
			}

			return ret;
		}

		[[nodiscard]] constexpr Vec transposed() const {
			Vec ret = *this;
			for(int i=0; i<NumElements/2; i++) {
				std::swap(ret[i], ret[NumElements-i-1]);
			}
			return ret;
		}

		template<typename TValue2>
		[[nodiscard]] constexpr Vec<TValue2, N> as() const {
			Vec<TValue2, N> ret;

			ret.forEachIndexed([&](TValue2& v, int i) {
				v = (*this)[i];
			});

			return ret;
		}


		[[nodiscard]] constexpr Vec<float, N> asFloat() const {
			return as<float>();
		}
		[[nodiscard]] constexpr Vec<double, N> asDouble() const {
			return as<double>();
		}
		[[nodiscard]] constexpr Vec<int, N> asInt() const {
			return as<int>();
		}
		[[nodiscard]] constexpr Vec<unsigned, N> asUInt() const {
			return as<unsigned>();
		}


		constexpr Vec() {
			fill(0);
		}

		constexpr explicit Vec(ValueType v) {
			fill(v);
		}


		/*
		 * ctor that initializes every value (Vec3f v = {1, 2, 3};)
		 * enables constructing from initializer lists without obnoxious casts
		 */
		template<std::convertible_to<ValueType>... TValues>
			requires (
				   sizeof...(TValues) == NumElements
				&& NumElements >= 2
			)
		constexpr Vec(TValues... values) {
			setValues(values...);
		}


		/* GLSL-like concatenation (Vec3f v1 = {1, 2}; Vec4f v2 = {v1, 3};) */
		template<typename SrcVecT, std::convertible_to<ValueType> TVal1, std::convertible_to<ValueType>... TValRest>
		requires (
			SrcVecT::NumElements + sizeof...(TValRest) + 1 == NumElements &&
			std::convertible_to<typename SrcVecT::ValueType, ValueType>
		)
		constexpr Vec(SrcVecT srcVec, TVal1 val1, TValRest... rest) {

			constexpr int VN = SrcVecT::NumElements;

			for(int i=0; i<VN; i++) {
				(*this)[i] = srcVec[i];
			}
			(*this)[VN] = val1;

			[&]<std::size_t... Idxs>([[maybe_unused]] std::index_sequence<Idxs...> seq){
				(((*this)[Idxs + VN + 1] = rest), ...);
			}(std::make_index_sequence<NumElements - VN - 1>{});
		}

		/*
		 * between-types conversion, implicit when non-narrowing
		 */
		template<typename OtherVecT>
		requires ((
			!std::same_as<ValueType, typename OtherVecT::ValueType> &&
			NumElements == OtherVecT::NumElements
		))
		explicit (IsNarrowingConversion<ValueType, typename OtherVecT::ValueType>)
		constexpr Vec(const OtherVecT& other)
			: Vec(other.template as<ValueType>())
		{

		}


		constexpr Vec(const Vec& rhs) = default;
		constexpr Vec& operator=(const Vec& rhs) = default;
		constexpr Vec(Vec&& rhs) noexcept = default;
		constexpr Vec& operator=(Vec&& rhs) noexcept = default;

	private:

		[[nodiscard]] constexpr Vec internalCross(const Vec& rhs) const {
			auto lr1 = rotated(1);
			auto lr2 = rotated(2);
			auto rr1 = rhs.rotated(1);
			auto rr2 = rhs.rotated(2);
			auto h1 = lr2.hadamard(rr1);
			auto h2 = lr1.hadamard(rr2);
			return h1 - h2;
		}
	};


	template<typename T>
	concept VectorType = requires{
		typename T::ValueType;
		{T::NumElements} -> std::convertible_to<int>;
	};
	
	template<typename T>
	using Vec2 = Vec<T, 2>;
	
	template<typename T>
	using Vec3 = Vec<T, 3>;
	
	template<typename T>
	using Vec4 = Vec<T, 4>;
	
	using Vec2d = Vec2<double>;
	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned>;
	using Vec2b = Vec2<uint8_t>;
	
	using Vec3d = Vec3<double>;
	using Vec3f = Vec3<float>;
	using Vec3i = Vec3<int>;
	using Vec3u = Vec3<unsigned>;
	using Vec3b = Vec3<uint8_t>;
	
	using Vec4d = Vec4<double>;
	using Vec4f = Vec4<float>;
	using Vec4i = Vec4<int>;
	using Vec4u = Vec4<unsigned>;
	using Vec4b = Vec4<uint8_t>;

}

#endif //AXXEGRO_VEC_HPP
