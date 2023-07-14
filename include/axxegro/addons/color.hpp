
#ifndef AXXEGRO_ADDONS_COLOR_HPP
#define AXXEGRO_ADDONS_COLOR_HPP

#include "../common.hpp"

#include "../core/gfx/Color.hpp"
#include <allegro5/allegro_color.h>

namespace al {
	
	namespace ColorCoord {

		struct RGB: public Vec3f {
			RGB(Color c) : Vec3f(c.r, c.g, c.b) {}
			Color toColor() {return {x, y, z, 1.0};};
		};

		struct RGBA: public Vec4f {
			RGBA(Color c) : Vec4f(c.r, c.g, c.b, c.a) {}
			Color toColor() {return {x, y, z, w};};
		};
		
		struct CMYK: public Vec4f {
			CMYK(Color c){al_color_rgb_to_cmyk(c.r, c.g, c.b, &x, &y, &z, &w);}
			Color toColor(){return al_color_cmyk(x, y, z, w);}
		};

#define AXXEGRO_COLOR_SPACE(clsname, fnfrom, fnto) struct clsname: public Vec3f { \
			clsname(Color c){fnfrom(c.r, c.g, c.b, &x, &y, &z);} \
			Color toColor() {return fnto(x, y, z);} \
		}
		
		AXXEGRO_COLOR_SPACE(HSL, al_color_rgb_to_hsl, al_color_hsl);
		AXXEGRO_COLOR_SPACE(HSV, al_color_rgb_to_hsv, al_color_hsv);
		AXXEGRO_COLOR_SPACE(XYZ, al_color_rgb_to_xyz, al_color_xyz);
		AXXEGRO_COLOR_SPACE(XYY, al_color_rgb_to_xyy, al_color_xyy);
		AXXEGRO_COLOR_SPACE(LAB, al_color_rgb_to_lab, al_color_lab);
		AXXEGRO_COLOR_SPACE(LCH, al_color_rgb_to_lch, al_color_lch);
		AXXEGRO_COLOR_SPACE(YUV, al_color_rgb_to_yuv, al_color_yuv);
		
		//for future 5.2.8 release
#if ALLEGRO_VERSION_INT >= AL_ID(5,2,8,1)
		AXXEGRO_COLOR_SPACE(Oklab, al_color_rgb_to_oklab, al_color_oklab);
		AXXEGRO_COLOR_SPACE(LinearRGB, al_color_rgb_to_linear, al_color_linear);
#endif
	
	}
}

#endif //AXXEGRO_ADDONS_COLOR_HPP
