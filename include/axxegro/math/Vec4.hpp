#ifndef INCLUDE_AXXEGRO_MATH_VEC4
#define INCLUDE_AXXEGRO_MATH_VEC4

#include <cmath>
#include <cstdint>
#include <functional>

namespace al {

	template<typename T = float>
	class Vec4 {
	public:
		static constexpr unsigned NumElements = 4;
		using ValueType = T;

		T x, y, z, w;

		constexpr Vec4():x(T(0)),y(T(0)),z(T(0)), w(T(0)){}
		constexpr Vec4(T x, T y, T z, T w):x(x),y(y),z(z),w(w){}

		template<typename U>
		constexpr Vec4(const Vec4<U>& v)
			: Vec4(v.x, v.y, v.z, v.w)
		{}
		
		constexpr Vec4& operator+=(const Vec4& rhs)
			{x+=rhs.x; y+=rhs.y; z+=rhs.z; w+=rhs.w; return *this;}
		constexpr Vec4& operator-=(const Vec4& rhs)
			{x-=rhs.x; y-=rhs.y; z-=rhs.z; w-=rhs.w; return *this;}
		constexpr Vec4& operator*=(T rhs)
			{x*=rhs; y*=rhs; z*=rhs; w*=rhs; return *this;}
		constexpr Vec4& operator/=(T rhs)
			{x/=rhs; y/=rhs; z/=rhs; w/=rhs; return *this;}

		constexpr Vec4 operator+() const 
			{return *this;}
		constexpr Vec4 operator-() const 
			{Vec4 ret; ret.x=-x; ret.y=-y; ret.z=-z; ret.w=-w; return ret;}

		constexpr Vec4 operator+(const Vec4& rhs) const 
			{Vec4 ret=*this; ret+=rhs; return ret;}
		constexpr Vec4 operator-(const Vec4& rhs) const 
			{Vec4 ret=*this; ret-=rhs; return ret;}
		constexpr Vec4 operator*(T rhs) const 
			{Vec4 ret=*this; ret*=rhs; return ret;}
		constexpr Vec4 operator/(T rhs) const 
			{Vec4 ret=*this; ret/=rhs; return ret;}

		constexpr bool operator==(const Vec4& rhs) const 
			{return x==rhs.x && y==rhs.y && z==rhs.z;}
		constexpr bool operator!=(const Vec4& rhs) const 
			{return !(*this==rhs);}
		

		constexpr Vec4 hadamard(const Vec4& rhs)
			{return Vec4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);}

		constexpr T length() const
			{return std::sqrt(x*x + y*y + z*z + w*w);}
		constexpr T dist(const Vec4& rhs) const
			{return (*this - rhs).length();}
		
		Vec4& apply(std::function<T(T)> fn)
		{
			x=fn(x); 
			y=fn(y); 
			z=fn(z);
			w=fn(w);
			return *this;
		}
		
		Vec4& floor()
		{
			return apply([](T v){
				return std::floor(v);
			});
		}
	};

	template<typename T = float>
	using Coord4 = Vec4<T>;

	using Vec4d = Vec4<double>;
	using Vec4f = Vec4<float>;
	using Vec4i = Vec4<int>;
	using Vec4u = Vec4<unsigned>;
	using Vec4b = Vec4<uint8_t>;
}

#endif /* INCLUDE_AXXEGRO_MATH_VEC4 */
