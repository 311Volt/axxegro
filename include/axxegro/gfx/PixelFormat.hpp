#ifndef INCLUDE_AXXEGRO_GFX_PIXELFORMAT
#define INCLUDE_AXXEGRO_GFX_PIXELFORMAT

#include "allegro5/color.h"
#include "axxegro/math/Vec2.hpp"
#include <allegro5/allegro5.h>

namespace al
{
	class PixelFormat
	{
	public:
		inline PixelFormat(ALLEGRO_PIXEL_FORMAT fmt):fmt(fmt){}
		inline operator ALLEGRO_PIXEL_FORMAT(){return fmt;}

		inline int size(){return al_get_pixel_size(fmt);}
		inline int bits(){return al_get_pixel_format_bits(fmt);}
		inline int blockSize(){return al_get_pixel_block_size(fmt);}
		inline int blockWidth(){return al_get_pixel_block_width(fmt);}
		inline int blockHeight(){return al_get_pixel_block_height(fmt);}
		inline Vec2i blockDimensions(){return {blockWidth(), blockHeight()};}
	private:
		ALLEGRO_PIXEL_FORMAT fmt;
	};
}

#endif /* INCLUDE_AXXEGRO_GFX_PIXELFORMAT */
