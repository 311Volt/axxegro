//
// Created by volt on 2023-07-18.
//

#ifndef AXXEGRO_TARGETBITMAP_HPP
#define AXXEGRO_TARGETBITMAP_HPP

#include "../Transform.hpp"
#include "Bitmap.hpp"

namespace al {


#define AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC [[maybe_unused]] auto* unused_____ = ptr();

	namespace detail {
		class CTargetBitmap: public Bitmap {
		public:
			CTargetBitmap() : Bitmap(nullptr) {}

			/**
			 * @brief Use the transform for subsequent drawing operations
			 * on the target bitmap.
			 * https://liballeg.org/a5docs/trunk/transformations.html#al_use_transform
			 */
			void useTransform(const Transform& transform) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_use_transform(&transform);
			}

			/**
			 * @brief Use the projection transformation for subsequent
			 * drawing operations on the target bitmap.
			 * https://liballeg.org/a5docs/trunk/transformations.html#al_use_projection_transform
			 */
			void useProjectionTransform(const Transform& transform) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_use_projection_transform(&transform);
			}

			void resetTransform() {
				useTransform(al::Transform::Identity());
			}
			void resetProjection() {
				useProjectionTransform(al::Transform::Orthographic({0,0}, size().as<float>(), -1, 1));
			}

			Transform currentTransform() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				return al_get_current_transform();
			}
			Transform currentInverseTransform() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				return al_get_current_inverse_transform();
			}
			Transform currentProjectionTransform() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				return al_get_current_projection_transform();
			}

			void setRenderState(ALLEGRO_RENDER_STATE state, int value) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_set_render_state(state, value);
			}
			void setAlphaTest(bool value) {
				setRenderState(ALLEGRO_ALPHA_TEST, value);
			}
			void setAlphaFunction(ALLEGRO_RENDER_FUNCTION value) {
				setRenderState(ALLEGRO_ALPHA_FUNCTION, value);
			}
			void setAlphaTestValue(uint8_t value) {
				setRenderState(ALLEGRO_ALPHA_TEST_VALUE, value);
			}
			void setWriteMaskFlags(ALLEGRO_WRITE_MASK_FLAGS value) {
				setRenderState(ALLEGRO_WRITE_MASK, value);
			}
			void setDepthTest(bool value) {
				setRenderState(ALLEGRO_DEPTH_TEST, value);
			}
			void setDepthFunction(ALLEGRO_RENDER_FUNCTION value) {
				setRenderState(ALLEGRO_DEPTH_FUNCTION, value);
			}

			void clearDepthBuffer(float x) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_clear_depth_buffer(x);
			}
			void setClippingRectangle(Rect<int> r) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_set_clipping_rectangle(r.a.x, r.a.y, r.width(), r.height());
			}
			[[nodiscard]] RectI getClippingRectangle() const { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				int x,y,w,h;
				al_get_clipping_rectangle(&x, &y, &w, &h);
				return RectI::XYWH(x,y,w,h);
			}
			void resetClippingRectangle() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
				al_reset_clipping_rectangle();
			}

		private:
			[[nodiscard]] ALLEGRO_BITMAP* getPointer() const override {
				return al_get_target_bitmap();
			}
		};
	}



#undef AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC

	inline detail::CTargetBitmap TargetBitmap;

#endif //AXXEGRO_TARGETBITMAP_HPP


}
