
#ifndef AXXEGRO_BLENDER_HPP
#define AXXEGRO_BLENDER_HPP

#include <allegro5/allegro.h>
#include "Color.hpp"

namespace al {
	
	struct Blender {
		int op = ALLEGRO_ADD;
		int src = ALLEGRO_ONE;
		int dst = ALLEGRO_INVERSE_ALPHA;
	};
	
	struct SeparateBlender {
		Blender color, alpha;
	};
	
	inline Blender GetBlender() {
		Blender ret;
		al_get_blender(&ret.op, &ret.src, &ret.dst);
		return ret;
	}
	
	inline void SetBlender(Blender blender) {
		al_set_blender(blender.op, blender.src, blender.dst);
	}
	
	inline SeparateBlender GetSeparateBlender() {
		SeparateBlender ret;
		al_get_separate_blender(&ret.color.op, &ret.color.src, &ret.color.dst,
								&ret.alpha.op, &ret.alpha.src, &ret.alpha.dst);
		return ret;
	}
	
	inline void SetSeparateBlender(SeparateBlender blender) {
		al_set_separate_blender(blender.color.op, blender.color.src, blender.color.dst,
								blender.alpha.op, blender.alpha.src, blender.alpha.dst);
		
	}
	
	struct ScopedBlender {
		explicit ScopedBlender(Blender blender): original(GetBlender()) {
			SetBlender(blender);
		}
		~ScopedBlender() {
			SetBlender(original);
		}
	private:
		Blender original;
	};
	
	struct ScopedSeparateBlender {
		explicit ScopedSeparateBlender(SeparateBlender blender): original(GetSeparateBlender()) {
			SetSeparateBlender(blender);
		}
		~ScopedSeparateBlender() {
			SetSeparateBlender(original);
		}
	private:
		SeparateBlender original;
	};
	
	
	inline Color GetBlendColor() {
		return al_get_blend_color();
	}
	
	inline Color SetBlendColor(Color color) {
		al_set_blend_color(color);
	}
	
}

#endif //AXXEGRO_BLENDER_HPP
