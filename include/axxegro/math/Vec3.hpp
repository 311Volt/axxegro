#ifndef INCLUDE_AXXEGRO_MATH_VEC3
#define INCLUDE_AXXEGRO_MATH_VEC3

#include <cmath>

namespace al {

	template<typename T = float>
	class Vec3 {
	public:
		T x, y, z;

		constexpr Vec3():x(T(0)),y(T(0)),z(T(0)){}
		constexpr Vec3(T x, T y, T z):x(x),y(y),z(z){}

		template<typename U>
		constexpr Vec3(const Vec3<U>& v)
			: Vec3(v.x, v.y, v.z)
		{}
		
		constexpr Vec3& operator+=(const Vec3& rhs)
			{x+=rhs.x; y+=rhs.y; z+=rhs.z; return *this;}
		constexpr Vec3& operator-=(const Vec3& rhs)
			{x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this;}
		constexpr Vec3& operator*=(T rhs)
			{x*=rhs; y*=rhs; z*=rhs; return *this;}
		constexpr Vec3& operator/=(T rhs)
			{x/=rhs; y/=rhs; z/=rhs; return *this;}

		constexpr Vec3 operator+() const 
			{return *this;}
		constexpr Vec3 operator-() const 
			{Vec3 ret; ret.x=-x; ret.y=-y; ret.z=-z; return ret;}

		constexpr Vec3 operator+(const Vec3& rhs) const 
			{Vec3 ret=*this; ret+=rhs; return ret;}
		constexpr Vec3 operator-(const Vec3& rhs) const 
			{Vec3 ret=*this; ret-=rhs; return ret;}
		constexpr Vec3 operator*(T rhs) const 
			{Vec3 ret=*this; ret*=rhs; return ret;}
		constexpr Vec3 operator/(T rhs) const 
			{Vec3 ret=*this; ret/=rhs; return ret;}

		constexpr bool operator==(const Vec3& rhs) const 
			{return x==rhs.x && y==rhs.y && z==rhs.z;}
		constexpr bool operator!=(const Vec3& rhs) const 
			{return !(*this==rhs);}
		
		constexpr T length() const
			{return std::sqrt(x*x + y*y + z*z);}
		constexpr T dist(const Vec3& rhs) const
			{return (*this - rhs).length();}
	};

	template<typename T>
	using Coord3 = Vec3<T>;
}

#endif /* INCLUDE_AXXEGRO_MATH_VEC3 */
