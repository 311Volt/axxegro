#ifndef INCLUDE_AXXEGRO_MATH_VEC2
#define INCLUDE_AXXEGRO_MATH_VEC2

#include <cmath>

namespace al {

	template<typename T>
	class Vec2 {
	public:
		T x, y;

		constexpr Vec2():x(0.0f),y(0.0f){}
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
	};

	template<typename T>
	using Coord = Vec2<T>;
}

#endif /* INCLUDE_AXXEGRO_MATH_POINT */
