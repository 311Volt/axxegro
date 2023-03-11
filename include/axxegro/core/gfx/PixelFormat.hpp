#ifndef INCLUDE_AXXEGRO_GFX_PIXELFORMAT
#define INCLUDE_AXXEGRO_GFX_PIXELFORMAT

#include "../../common.hpp"

namespace al
{
	class PixelFormat
	{
	public:
		PixelFormat(ALLEGRO_PIXEL_FORMAT fmt) :fmt(fmt){}
		operator ALLEGRO_PIXEL_FORMAT() {
			return fmt;
		}

		int size() {
			return al_get_pixel_size(fmt);
		}
		int bits() {
			return al_get_pixel_format_bits(fmt);
		}
		int blockSize() {
			return al_get_pixel_block_size(fmt);
		}
		int blockWidth() {
			return al_get_pixel_block_width(fmt);
		}
		int blockHeight() {
			return al_get_pixel_block_height(fmt);
		}
		Vec2i blockDimensions() {
			return {blockWidth(), blockHeight()};
		}
	private:
		ALLEGRO_PIXEL_FORMAT fmt;
	};
}

#endif /* INCLUDE_AXXEGRO_GFX_PIXELFORMAT */
