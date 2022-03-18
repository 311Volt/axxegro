#ifndef INCLUDE_AXXEGRO_MATH_POINT
#define INCLUDE_AXXEGRO_MATH_POINT

#include <cmath>

namespace al {
	class Point {
	public:
		float x, y;

		constexpr Point():x(0.0f),y(0.0f){}
		constexpr Point(float x, float y):x(x),y(y){}
		constexpr Point(int x, int y):x(float(x)),y(float(y)){}
		
		constexpr Point& operator+=(const Point& rhs)
			{x+=rhs.x; y+=rhs.y; return *this;}
		constexpr Point& operator-=(const Point& rhs)
			{x-=rhs.x; y-=rhs.y; return *this;}
		constexpr Point& operator*=(float rhs)
			{x*=rhs; y*=rhs; return *this;}
		constexpr Point& operator/=(float rhs)
			{x/=rhs; y/=rhs; return *this;}

		constexpr Point operator+() const 
			{return *this;}
		constexpr Point operator-() const 
			{Point ret; ret.x=-x; ret.y=-y; return ret;}

		constexpr Point operator+(const Point& rhs) const 
			{Point ret=*this; ret+=rhs; return ret;}
		constexpr Point operator-(const Point& rhs) const 
			{Point ret=*this; ret-=rhs; return ret;}
		constexpr Point operator*(float rhs) const 
			{Point ret=*this; ret*=rhs; return ret;}
		constexpr Point operator/(float rhs) const 
			{Point ret=*this; ret/=rhs; return ret;}

		constexpr bool operator==(const Point& rhs) const 
			{return x==rhs.x && y==rhs.y;}
		constexpr bool operator!=(const Point& rhs) const 
			{return !(*this==rhs);}
		
		constexpr float hypot() const
			{return std::sqrt(x*x + y*y);}
		constexpr float dist(const Point& rhs) const
			{return (*this - rhs).hypot();}
	};

	using Vec2 = Point;
}

#endif /* INCLUDE_AXXEGRO_MATH_POINT */
