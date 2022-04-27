#ifndef INCLUDE_AXXEGRO_COLOR
#define INCLUDE_AXXEGRO_COLOR

/**
 @file 
 a color class
*/

#include <cstdint>

#include <tuple>
#include <string_view>
#include <type_traits>

#include <allegro5/allegro.h>
#include <allegro5/color.h>

namespace al {

	//TODO literals 
	
	class Color: public ALLEGRO_COLOR {
	private:
		#define AXXEGRO_COLOR_ERR "axxegro expects ALLEGRO_COLOR to be the same as struct{float r,g,b,a;};"
		static_assert(std::is_same<float, decltype(r)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(g)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(b)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(a)>::value, AXXEGRO_COLOR_ERR);	
		#undef AXXEGRO_COLOR_ERR
	public:

		constexpr Color(float r, float g, float b, float a)
			: ALLEGRO_COLOR()
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;	
		}
		constexpr Color(float r, float g, float b) 
			: Color(r,g,b,1.0f) 
		{}

		constexpr Color()
			: Color(0,0,0,0)
		{}

		/** Creates an opaque Color from a 32-bit number.
		    @param x A 32-bit number of the form 0xXXRRGGBB (X=don't care).
		 */
		static constexpr al::Color U32_RGB(uint32_t x)
			{return U32_RGBA(x | 0xFF000000);}
		
		/** Creates a Color from a 32-bit number.
		    @param x A 32-bit number of the form 0xAARRGGBB
		 */
		static constexpr al::Color U32_RGBA(uint32_t x)
		{
			uint8_t a = (x>>24) & 0xFF;
			uint8_t r = (x>>16) & 0xFF;
			uint8_t g = (x>>8) & 0xFF;
			uint8_t b = (x>>0) & 0xFF;
			
			return RGBA(r,g,b,a);
		}
		
		/// Creates a Color from three 8-bit channel values.
		static constexpr Color RGB(uint8_t r, uint8_t g, uint8_t b)
			{return RGBA(r, g, b, 255);}

		/// Creates a Color from four 8-bit channel values.
		static constexpr Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{return RGBA_f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);}

		/// Effectively RGBA(r*a, g*a, b*a, a). 
		static constexpr Color PremulRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{return PremulRGBA_f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);}
		
		/// Equivalent to Color(r,g,b).
		static constexpr Color RGB_f(float r, float g, float b)
			{return Color(r,g,b);}

		/// Equivalent to Color(r,g,b,a).
		static constexpr Color RGBA_f(float r, float g, float b, float a)
			{return Color(r,g,b,a);}
		
		/// Effectively RGBA(r*a, g*a, b*a, a). 
		static constexpr Color PremulRGBA_f(float r, float g, float b, float a)
			{return Color(r*a,g*a,b*a,a);}

		/** Returns all channel values as a tuple of floats.
		 *  Intended for use with C++17 structured bindings. */
		constexpr std::tuple<float,float,float,float> rgbaF() const
			{return {r, g, b, a};}

		/** Returns all channel values as a tuple of bytes.
		 *  Intended for use with C++17 structured bindings. */
		constexpr std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> rgbaU8() const
			{return {r*255.0f, g*255.0f, b*255.0f, a*255.0f};}
		
		///@returns The color as a 32-bit int of the form 0xXXRRGGBB (X=don't care).
		constexpr uint32_t rgb_u32() const
		{
			uint32_t ret = 0;
			const auto& [r,g,b,a] = rgbaU8();
			ret += (uint32_t)r << 16;
			ret += (uint32_t)g << 8;
			ret += (uint32_t)b << 0;
			return ret;
		}

		///@returns The color as a 32-bit int of the form 0xAARRGGBB.
		constexpr uint32_t rgba_u32() const
		{
			uint32_t ret = 0;
			const auto& [r,g,b,a] = rgbaU8();
			ret += (uint32_t)a << 24;
			ret += (uint32_t)r << 16;
			ret += (uint32_t)g << 8;
			ret += (uint32_t)b << 0;
			return ret;
		}

		Color(ALLEGRO_COLOR color)
			{al_unmap_rgba_f(color,&r,&g,&b,&a);}

		ALLEGRO_COLOR operator()() const
			{return al_map_rgba_f(r,g,b,a);}
		
		ALLEGRO_COLOR get() const
			{return operator()();}
	};

	///@brief Color constants
	namespace Col {
		constexpr Color Black =   Color::U32_RGB(0x000000);
		constexpr Color Blue =    Color::U32_RGB(0x0000FF);
		constexpr Color Green =   Color::U32_RGB(0x00FF00);
		constexpr Color Cyan =    Color::U32_RGB(0x00FFFF);
		constexpr Color Red =     Color::U32_RGB(0xFF0000);
		constexpr Color Magenta = Color::U32_RGB(0xFF00FF);
		constexpr Color Yellow =  Color::U32_RGB(0xFFFF00);
		constexpr Color White =   Color::U32_RGB(0xFFFFFF);
		
	}

	
}

#endif /* INCLUDE_AXXEGRO_COLOR */
