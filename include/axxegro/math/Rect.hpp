#ifndef INCLUDE_AXXEGRO_MATH_RECT
#define INCLUDE_AXXEGRO_MATH_RECT

#include "Point.hpp"

namespace al {
	class Rect {
	public:
		Point a, b;

		Rect(){} // = {Point(),Point()} = {{0,0},{0,0}}
		constexpr Rect(const Point& a, const Point& b)
			:a(a),b(b){}
		constexpr Rect(float x1, float y1, float x2, float y2)
			:Rect(Point(x1,y1), Point(x2,y2)){}

		constexpr float getWidth() const 
			{return b.x-a.x;}
		constexpr float getHeight() const 
			{return b.y-a.y;}

		constexpr Point topLeft() const 
			{return a;}
		constexpr Point bottomLeft() const 
			{return {a.x, b.y};}
		constexpr Point topRight() const 
			{return {a.y, b.x};}
		constexpr Point topBottom() const 
			{return b;}

		constexpr Vec2 getSize() const 
			{return Vec2(getWidth(),getHeight());}

		constexpr bool operator==(const Rect& rhs) const 
			{return a==rhs.a && b==rhs.b;}
		constexpr bool operator!=(const Rect& rhs) const 
			{return !(*this==rhs);}
		
		constexpr bool contains(Point p) const
		{
			return p.x >= a.x && p.x <= b.x && p.y >= a.y && p.y <= b.y;
		}

		constexpr bool contains(Rect r) const
		{
			return contains(r.a) && contains(r.b);
		}
	};
}

#endif /* INCLUDE_AXXEGRO_MATH_RECT */
	