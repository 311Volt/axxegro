#ifndef INCLUDE_AXXEGRO_MATH_RECT
#define INCLUDE_AXXEGRO_MATH_RECT

#include "Vec2.hpp"

namespace al {

	template<typename T>
	class Rect {
	public:
		Vec2<T> a, b;

		Rect(){} // = {Vec2<T>(),Vec2<T>()} = {{0,0},{0,0}}
		constexpr Rect(const Vec2<T>& a, const Vec2<T>& b)
			:a(a),b(b){}
		constexpr Rect(float x1, float y1, float x2, float y2)
			:Rect(Vec2<T>(x1,y1), Vec2<T>(x2,y2))
		{}

		template<typename U>
		constexpr Rect(const Rect<U>& r)
			: Rect(r.a, r.b)
		{}

		constexpr float width() const 
			{return b.x-a.x;}
		constexpr float height() const 
			{return b.y-a.y;}

		constexpr Vec2<T> topLeft() const 
			{return a;}
		constexpr Vec2<T> bottomLeft() const 
			{return {a.x, b.y};}
		constexpr Vec2<T> topRight() const 
			{return {a.y, b.x};}
		constexpr Vec2<T> topBottom() const 
			{return b;}

		constexpr Vec2<T> size() const 
			{return Vec2<T>(width(),height());}

		constexpr bool operator==(const Rect& rhs) const 
			{return a==rhs.a && b==rhs.b;}
		constexpr bool operator!=(const Rect& rhs) const 
			{return !(*this==rhs);}
		
		constexpr bool contains(Vec2<T> p) const
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
	