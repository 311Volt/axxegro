#ifndef INCLUDE_AXXEGRO_COLOR
#define INCLUDE_AXXEGRO_COLOR

/**
 @file 
 a color class
*/

#include "../../common.hpp"

namespace al {

	
	class Color: public ALLEGRO_COLOR {
	private:
		#define AXXEGRO_COLOR_ERR "axxegro expects ALLEGRO_COLOR to be the same as struct{float r,g,b,a;};"
		static_assert(std::is_same<float, decltype(r)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(g)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(b)>::value, AXXEGRO_COLOR_ERR);	
		static_assert(std::is_same<float, decltype(a)>::value, AXXEGRO_COLOR_ERR);	
		#undef AXXEGRO_COLOR_ERR
	public:

		constexpr Color(float r, float g, float b, float a=1.0f)
			: ALLEGRO_COLOR()
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;	
		}

		constexpr Color()
			: Color(0,0,0,0)
		{}

		Color(const ALLEGRO_COLOR& c)
		{
			this->r = c.r;
			this->g = c.g;
			this->b = c.b;
			this->a = c.a;
		}

		/** Returns all channel values as a tuple of bytes. */
		constexpr std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> rgbaU8() const
		{
			return {r*255.0f, g*255.0f, b*255.0f, a*255.0f};
		}
		
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
	};

	
	/// @brief Equivalent to Color(r,g,b).
	constexpr Color RGB_f(float r, float g, float b)
	{
		return Color(r,g,b);
	}

	/// @brief Equivalent to Color(r,g,b,a).
	constexpr Color RGBA_f(float r, float g, float b, float a)
	{
		return Color(r,g,b,a);
	}
	
	/// @brief Effectively RGBA(r*a, g*a, b*a, a). 
	constexpr Color PremulRGBA_f(float r, float g, float b, float a)
	{
		return Color(r*a,g*a,b*a,a);
	}

	/// @brief Creates a Color from four 8-bit channel values.
	constexpr Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		constexpr float f_1_255 = 1.0f / 255.0f;
		return RGBA_f(r*f_1_255, g*f_1_255, b*f_1_255, a*f_1_255);
	}
	
	/// @brief Creates a Color from three 8-bit channel values.
	constexpr Color RGB(uint8_t r, uint8_t g, uint8_t b)
	{
		return RGBA(r, g, b, 255);
	}

	/** Creates a Color from a 32-bit number.
	  * @param x A 32-bit number of the form 0xAARRGGBB */
	constexpr al::Color U32_RGBA(uint32_t x)
	{
		uint8_t a = (x>>24) & 0xFF;
		uint8_t r = (x>>16) & 0xFF;
		uint8_t g = (x>>8) & 0xFF;
		uint8_t b = (x>>0) & 0xFF;
		
		return RGBA(r,g,b,a);
	}

	/** Creates an opaque Color from a 32-bit number.
	  * @param x A 32-bit number of the form 0xXXRRGGBB (X=don't care). */
	constexpr al::Color U32_RGB(uint32_t x)
	{
		return U32_RGBA(x | 0xFF000000);
	}
	

	/// @brief Effectively RGBA(r*a, g*a, b*a, a). 
	constexpr Color PremulRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		constexpr float f_1_255 = 1.0f / 255.0f;
		return PremulRGBA_f(r*f_1_255, g*f_1_255, b*f_1_255, a*f_1_255);
	}

	constexpr Color Gray(float lum)
	{
		lum = std::clamp(lum, 0.0f, 1.0f);
		return RGB_f(lum, lum, lum);
	}

	constexpr Color Gray8(uint8_t lum)
	{
		return RGB(lum, lum, lum);
	}
	
	
	/** @brief Creates the nth color from the 16-color CGA palette.
	 * The upper 4 bits are ignored. */
	constexpr Color CGA(uint8_t c)
	{
		if(c==6) {
			return U32_RGB(0xAA5500);
		}
		uint32_t b = -((c&1) != 0);
		uint32_t g = -((c&2) != 0);
		uint32_t r = -((c&4) != 0);
		uint32_t i = -((c&8) != 0);
		return U32_RGB((r&0xAA0000) | (g&0x00AA00) | (b&0x0000AA) | (i&0x555555));
	}

	inline namespace Colors
	{
		constexpr Color Black = CGA(0);
		constexpr Color Blue = CGA(1);
		constexpr Color Green = CGA(2);
		constexpr Color Cyan = CGA(3);
		constexpr Color Red = CGA(4);
		constexpr Color Magenta = CGA(5);
		constexpr Color Brown = CGA(6);
		constexpr Color LightGray = CGA(7);
		
		constexpr Color DarkGray = CGA(8);
		constexpr Color LightBlue = CGA(9);
		constexpr Color LightGreen = CGA(10);
		constexpr Color LightCyan = CGA(11);
		constexpr Color LightRed = CGA(12);
		constexpr Color LightMagenta = CGA(13);
		constexpr Color Yellow = CGA(14);
		constexpr Color White = CGA(15);
		
		constexpr Color PureBlue =    U32_RGB(0x0000FF);
		constexpr Color PureGreen =   U32_RGB(0x00FF00);
		constexpr Color PureCyan =    U32_RGB(0x00FFFF);
		constexpr Color PureRed =     U32_RGB(0xFF0000);
		constexpr Color PureMagenta = U32_RGB(0xFF00FF);
		constexpr Color PureYellow =  U32_RGB(0xFFFF00);
	}


	inline namespace Literals {
		inline namespace ColorLiterals {
			constexpr Color operator""_RGB(unsigned long long color)
			{
				return U32_RGB(color);
			}
		}
	}


}

#endif /* INCLUDE_AXXEGRO_COLOR */
