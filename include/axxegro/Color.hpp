#ifndef INCLUDE_AXXEGRO_COLOR
#define INCLUDE_AXXEGRO_COLOR

/**
 @file 
 a color class
*/

#include <allegro5/allegro.h>
#include <cstdint>

#include <tuple>
#include <string_view>

namespace al {
	class Color {
	private:
		float r,g,b,a;
	public:
		constexpr Color(float r, float g, float b, float a)
			: r(r), g(g), b(b), a(a)
		{}
		constexpr Color(float r, float g, float b) 
			: Color(r,g,b,1.0f) 
		{}

		/** @brief Creates an opaque Color from a 32-bit number.
		  * @param x A 32-bit number of the form 0xXXRRGGBB (X=don't care). */
		static constexpr al::Color U32_RGB(uint32_t x)
			{return U32_RGBA(x | 0xFF000000);}
		
		/** @brief Creates a Color from a 32-bit number.
		  * @param x A 32-bit number of the form 0xAARRGGBB */
		static constexpr al::Color U32_RGBA(uint32_t x)
		{
			uint8_t r = (x>>16) & 0xFF;
			uint8_t g = (x>>8) & 0xFF;
			uint8_t b = (x>>0) & 0xFF;
			uint8_t a = (x>>24) & 0xFF;
			
			return RGBA(r,g,b,a);
		}
		
		/// @brief Creates a Color from three 8-bit channel values.
		static constexpr Color RGB(uint8_t r, uint8_t g, uint8_t b)
			{return RGBA(r, g, b, 255);}

		/// @brief Creates a Color from four 8-bit channel values.
		static constexpr Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{return RGBA_f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);}

		/// @brief Effectively RGBA(r*a, g*a, b*a, a). 
		static constexpr Color PremulRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{return PremulRGBA_f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);}
		
		/// @brief Equivalent to Color(r,g,b).
		static constexpr Color RGB_f(float r, float g, float b)
			{return Color(r,g,b);}

		/// @brief Equivalent to Color(r,g,b,a).
		static constexpr Color RGBA_f(float r, float g, float b, float a)
			{return Color(r,g,b,a);}
		
		/// @brief Effectively RGBA(r*a, g*a, b*a, a). 
		static constexpr Color PremulRGBA_f(float r, float g, float b, float a)
			{return Color(r*a,g*a,b*a,a);}
		
		/**
		 * @return The n-th color from the CGA palette. bc why not <3
		 * The behavior is unspecified if a number greater than 15 is given.
		 */
		static constexpr Color CGA(uint8_t rgbi)
		{
			if(rgbi == 6)
				return U32_RGB(0xAA5500);
			return U32_RGB(
				((rgbi&8) >> 3) * 0x555555 + //i 
				((rgbi&4) >> 2) * 0xAA0000 + //r
				((rgbi&2) >> 1) * 0x00AA00 + //g
				((rgbi&1) >> 0) * 0x0000AA   //b
			);

		}

		/**
		 * @return One of 8 colors from the r1g1b1 palette.
		 * The behavior is unspecified if a number greater than 7 is given.
		 */
		static constexpr Color R1G1B1(uint8_t color)
		{
			return U32_RGB(
				((color&4) >> 2) * 0xFF0000 + //r
				((color&2) >> 1) * 0x00FF00 + //g
				((color&1) >> 0) * 0x0000FF   //b
			);
		}

		/** @returns all channel values as a tuple of floats.
		 *  Intended for use with C++17 structured bindings. */
		constexpr std::tuple<float,float,float,float> rgbaF() const
			{return {r, g, b, a};}

		/** @returns all channel values as a tuple of bytes.
		 *  Intended for use with C++17 structured bindings. */
		constexpr std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> rgbaU8() const
			{return {r*255.0f, g*255.0f, b*255.0f, a*255.0f};}

		Color(ALLEGRO_COLOR color)
			{al_unmap_rgba_f(color,&r,&g,&b,&a);}

		ALLEGRO_COLOR operator()() const
			{return al_map_rgba_f(r,g,b,a);}
		
		ALLEGRO_COLOR get() const
			{return operator()();}
		
	};

	
}

#endif /* INCLUDE_AXXEGRO_COLOR */
