#ifndef INCLUDE_AXXEGRO_MATH_VEC2
#define INCLUDE_AXXEGRO_MATH_VEC2

#include <cmath>
#include <cstdint>
#include <functional>

namespace al {

	template<typename T = float>
	class Vec2 {
	public:
		static constexpr unsigned NumElements = 2;
		using ValueType = T;

		T x, y;

		constexpr Vec2():x(T(0)),y(T(0)){}
		constexpr Vec2(T x, T y):x(x),y(y){}

		template<typename U>
		constexpr Vec2(const Vec2<U>& v)
			: Vec2(v.x, v.y)
		{}
		
		constexpr Vec2& operator+=(const Vec2& rhs)
			{x+=rhs.x; y+=rhs.y; return *this;}
		constexpr Vec2& operator-=(const Vec2& rhs)
			{x-=rhs.x; y-=rhs.y; return *this;}
		constexpr Vec2& operator*=(T rhs)
			{x*=rhs; y*=rhs; return *this;}
		constexpr Vec2& operator/=(T rhs)
			{x/=rhs; y/=rhs; return *this;}

		constexpr Vec2 operator+() const 
			{return *this;}
		constexpr Vec2 operator-() const 
			{Vec2 ret; ret.x=-x; ret.y=-y; return ret;}

		constexpr Vec2 operator+(const Vec2& rhs) const 
			{Vec2 ret=*this; ret+=rhs; return ret;}
		constexpr Vec2 operator-(const Vec2& rhs) const 
			{Vec2 ret=*this; ret-=rhs; return ret;}
		constexpr Vec2 operator*(T rhs) const 
			{Vec2 ret=*this; ret*=rhs; return ret;}
		constexpr Vec2 operator/(T rhs) const 
			{Vec2 ret=*this; ret/=rhs; return ret;}

		constexpr bool operator==(const Vec2& rhs) const 
			{return x==rhs.x && y==rhs.y;}
		constexpr bool operator!=(const Vec2& rhs) const 
			{return !(*this==rhs);}
		
		constexpr T length() const
			{return std::sqrt(x*x + y*y);}
		constexpr T dist(const Vec2& rhs) const
			{return (*this - rhs).length();}
		
		constexpr Vec2 normalized() const
		{
			return (*this) / this->length();
		}

		constexpr Vec2 hadamard(const Vec2& rhs)
			{return Vec2(x*rhs.x, y*rhs.y);}

		constexpr T dot(const Vec2& rhs)
			{return x*rhs.x + y*rhs.y;}
		
		Vec2& apply(std::function<T(T)> fn)
		{
			x=fn(x); 
			y=fn(y); 
			return *this;
		}
		
		Vec2& floor()
		{
			return apply([](T v){
				return std::floor(v);
			});
		}

		Vec2 transposed()
		{
			return Vec2<T>(y, x);
		}
	};

	template<typename T = float>
	using Coord = Vec2<T>;

	template<typename T = float>
	using Coord2 = Vec2<T>;

	using CoordF = Coord<float>;
	using CoordD = Coord<double>;
	using CoordI = Coord<int32_t>;

	using Vec2d = Vec2<double>;
	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned>;
	using Vec2b = Vec2<uint8_t>;
	
}

#endif /* INCLUDE_AXXEGRO_MATH_VEC3 */
