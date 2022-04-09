#ifndef INCLUDE_AXXEGRO_MATH_RECT
#define INCLUDE_AXXEGRO_MATH_RECT

#include "Vec2.hpp"
#include <algorithm>
#include <cstdint>

namespace al {

	template<typename T = float>
	class Rect {
	public:
		Vec2<T> a, b;

		Rect(){} // = {Vec2<T>(),Vec2<T>()} = {{0,0},{0,0}}
		constexpr Rect(const Vec2<T>& a, const Vec2<T>& b)
			:a(a),b(b){}
		constexpr Rect(float x1, float y1, float x2, float y2)
			:Rect(Vec2<T>(x1,y1), Vec2<T>(x2,y2))
		{}

		constexpr static Rect PosSize(const Vec2<T>& pos, const Vec2<T>& size)
		{
			return Rect(pos, pos+size);
		}

		constexpr static Rect XYWH(T x, T y, T w, T h)
		{
			return PosSize({x,y}, {w,h});
		}

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
			{return {b.x, a.y};}
		constexpr Vec2<T> bottomRight() const 
			{return b;}

		constexpr Vec2<T> size() const 
			{return Vec2<T>(width(),height());}

		constexpr Vec2<T> center() const
			{return a + size()/T(2);}
		
		constexpr Rect& operator+=(const Vec2<T> vec)
			{a += vec; b += vec; return *this;}
		constexpr Rect& operator-=(const Vec2<T> vec)
			{a -= vec; b -= vec; return *this;}
		constexpr Rect& operator*=(const T rhs)
			{b = a + size()*rhs; return *this;}
		constexpr Rect& operator/=(const T rhs)
			{b = a + size()/rhs; return *this;}
		
		
		constexpr Rect operator+(const Vec2<T>& rhs) const 
			{Rect ret=*this; ret+=rhs; return ret;}
		constexpr Rect operator-(const Vec2<T>& rhs) const 
			{Rect ret=*this; ret-=rhs; return ret;}
		constexpr Rect operator*(T rhs) const 
			{Rect ret=*this; ret*=rhs; return ret;}
		constexpr Rect operator/(T rhs) const 
			{Rect ret=*this; ret/=rhs; return ret;}

		constexpr bool operator==(const Rect& rhs) const 
			{return a==rhs.a && b==rhs.b;}
		constexpr bool operator!=(const Rect& rhs) const 
			{return !(*this==rhs);}
		
		constexpr T area()
		{
			return width()*height();
		}
		

		constexpr Rect& limit(const al::Vec2<T>& minSize)
		{
			b.x = std::max(b.x, a.x+minSize.x);
			b.y = std::max(b.y, a.y+minSize.y);
			return *this;
		}

		template<typename U>
		constexpr U areaAs()
		{
			return U(width()) * U(height());
		}

		constexpr bool contains(Vec2<T> p) const
		{
			return p.x >= a.x && p.x <= b.x && p.y >= a.y && p.y <= b.y;
		}

		constexpr bool contains(const Rect& r) const
		{
			return contains(r.a) && contains(r.b);
		}

		/**
		 * @return a bitmask with the following meaning:
		 *     0x01 - the rectangle has a non-empty intersection with {P in R^2: x < a.x}
		 *     0x02 - the rectangle has a non-empty intersection with {P in R^2: x > b.x}
		 *     0x04 - the rectangle has a non-empty intersection with {P in R^2: y < a.y}
		 *     0x08 - the rectangle has a non-empty intersection with {P in R^2: y > b.y}
		 * a return value of 0 means that *this contains inner.
		 */
		constexpr uint8_t test(const Rect& inner) const
		{
			return (
				(!!(inner.a.x < a.x) << 0) | 
				(!!(inner.b.x > b.x) << 1) | 
				(!!(inner.a.y < a.y) << 2) | 
				(!!(inner.b.y > b.y) << 3)
			);
		}

		//everything in here desperately needs to be renamed by someone who can into english

		//test bitfield constants
		static constexpr uint8_t TEST_X_TOO_LOW = 0x01;
		static constexpr uint8_t TEST_X_TOO_HIGH = 0x02;
		static constexpr uint8_t TEST_X_NOT_IN_RANGE = 0x03;
		
		static constexpr uint8_t TEST_Y_TOO_LOW = 0x04;
		static constexpr uint8_t TEST_Y_TOO_HIGH = 0x08;
		static constexpr uint8_t TEST_Y_NOT_IN_RANGE = 0x0C;
		
		///@returns a point that is clipped to the rectangle's area
		constexpr Coord<T> clip(const Coord<T>& point) const
		{
			return {
				std::clamp(point.x, a.x, b.x),
				std::clamp(point.y, a.y, b.y)
			};
		}

		/**
		 * @returns the part of other that is contained in *this (can be empty) 
		 */
		constexpr Rect intersect(const Rect& outer) const
		{
			return {clip(outer.a), clip(outer.b)};
		}

		/** 
		 * @returns a rectangle that is just large enough to contain *this and other
		 * i can't name this "union" bc that is a keyword :(
		 */
		constexpr Rect makeUnion(const Rect& other) const
		{
			return Rect(
				std::min(a.x, other.a.x),
				std::min(a.y, other.a.y),
				std::max(b.x, other.b.x),
				std::max(b.y, other.b.y)
			);
		}

		constexpr Rect clamp(const Rect& inner) const
		{
			Rect ret = inner;
			uint8_t t = test(inner);
			if(t & TEST_X_TOO_LOW) {
				ret += {a.x - inner.a.x, T(0)};
			}
			if(t & TEST_X_TOO_HIGH) {
				ret -= {inner.b.x - b.x, T(0)};
			}
			if(t & TEST_Y_TOO_LOW) {
				ret += {T(0), a.y - inner.a.y};
			}
			if(t & TEST_Y_TOO_HIGH) {
				ret -= {T(0), inner.b.y - b.y};
			}
			return ret;
		}
	};
}

#endif /* INCLUDE_AXXEGRO_MATH_RECT */
	