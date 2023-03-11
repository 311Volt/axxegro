#ifndef INCLUDE_AXXEGRO_RESOURCES_BITMAP
#define INCLUDE_AXXEGRO_RESOURCES_BITMAP

#include "../../common.hpp"
#include "Color.hpp"

#include "../Transform.hpp"

/**
 * @file
 * An ALLEGRO_BITMAP wrapper plus utilities.
 */

namespace al {
	AXXEGRO_DEFINE_DELETER(ALLEGRO_BITMAP, al_destroy_bitmap);

	class BitmapLockedRegion;
	class Bitmap;

	/**
	 * @brief Provides a RAII-style mechanism for setting target bitmaps.
	 */
	class ScopedTargetBitmap {
	private:
		ALLEGRO_BITMAP* oldTarget;
	public:
		explicit ScopedTargetBitmap(ALLEGRO_BITMAP* bmp)
		{
			oldTarget = al_get_target_bitmap();
			al_set_target_bitmap(bmp);
		}
		explicit ScopedTargetBitmap(Bitmap& bmp);

		~ScopedTargetBitmap()
		{
			al_set_target_bitmap(oldTarget);
		}
	};


	/**
	 * @brief Wraps around ALLEGRO_BITMAP.
	 */
	class Bitmap: public Resource<ALLEGRO_BITMAP> {
	public:
		using Resource::Resource;
		Bitmap() = delete;
		
		/** @brief Creates a bitmap.
		 * @param w Width in pixels.
		 * @param h Height in pixels. 
		 **/
		Bitmap(int w, int h)
				: Resource(al_create_bitmap(w,h))
		{
			if(!ptr()) {
				throw ResourceLoadError("Error while creating a %dx%d bitmap", w, h);
			}
		}

		/**
		 * @brief Creates a bitmap that is pre-cleared to a given color.
		 */
		Bitmap(int w, int h, Color color)
				: Bitmap(w,h)
		{
			clearToColor(color);
		}

		/** 
		 * @brief Loads a bitmap from a file. 
		 * @param filename Path to the file with the image.
		 **/
		explicit Bitmap(const std::string& filename)
				: Resource(al_load_bitmap(filename.c_str()))
		{
			if(!ptr()) {
				throw ResourceLoadError("Cannot load bitmap from %s - file missing, corrupted or in an unsupported format", filename.c_str());
			}
		}

		/// @return The width of the bitmap in pixels.
		[[nodiscard]] int width() const {
			return al_get_bitmap_width(ptr());
		}

		/// @return The height of the bitmap in pixels.
		[[nodiscard]] int height() const {
			return al_get_bitmap_height(ptr());
		}

		/// @return The dimensions (width x height) of the bitmap in pixels.
		[[nodiscard]] Vec2<int> size() const {
			return {width(), height()};
		}

		/**
		 * @return A rectangle spanning the entire bitmap, with the top left corner at (0, 0)
		 * and the bottom left corner at (width, height). Useful for the drawScaled() family
		 * of methods.
		 **/
		[[nodiscard]] Rect<int> rect() const {
			return {{0, 0}, size()};
		}

		/* note: I left out the flags parameter. al_draw_xxx_bitmap is
		   always called with flags=0. use transforms/texcoords for flipping. */

		/// @brief Corresponds to al_draw_bitmap(). Refer to Allegro5 documentation for more info on this and other drawX() methods.
		void draw(Coord<float> p0) const {
			al_draw_bitmap(ptr(), p0.x, p0.y, 0);
		}

		void drawTinted(Coord<float> p0, Color tint) const {
			al_draw_tinted_bitmap(ptr(), tint, p0.x, p0.y, 0);
		}

		void drawRegion(Rect<float> srcRegion, Coord<float> dst) const {
			al_draw_bitmap_region(
					ptr(),
					srcRegion.a.x, srcRegion.a.y,
					srcRegion.width(), srcRegion.height(),
					dst.x, dst.y,
					0
			);
		}

		void drawScaled(Rect<float> srcRect, Rect<float> dstRect) const {
			al_draw_scaled_bitmap(
					ptr(),
					srcRect.a.x, srcRect.a.y,
					srcRect.width(), srcRect.height(),
					dstRect.a.x, dstRect.a.y,
					dstRect.width(), dstRect.height(),
					0
			);
		}
		void drawRotated(Coord<float> centerSrc, Coord<float> centerDst, float angle) const {
			al_draw_rotated_bitmap(
					ptr(),
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					angle,
					0
			);
		}

		void drawTintedScaled(Color tint, Rect<float> srcRect, Rect<float> dstRect) const {
			al_draw_tinted_scaled_bitmap(
					ptr(),
					tint,
					srcRect.a.x, srcRect.a.y,
					srcRect.width(), srcRect.height(),
					dstRect.a.x, dstRect.a.y,
					dstRect.width(), dstRect.height(),
					0
			);
		}

		void drawTintedRegion(Rect<float> srcRegion, Coord<float> dst, Color tint) const {
			al_draw_tinted_bitmap_region(
					ptr(),
					tint,
					srcRegion.a.x, srcRegion.a.y,
					srcRegion.width(), srcRegion.height(),
					dst.x, dst.y,
					0
			);
		}

		void drawTintedRotated(Color tint, Coord<float> centerSrc, Coord<float> centerDst, float angle) const {
			al_draw_tinted_rotated_bitmap(
					ptr(),
					tint,
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					angle,
					0
			);
		}

		void drawScaledRotated(Coord<float> centerSrc, Coord<float> centerDst, Vec2<float> scale, float angle) const {
			al_draw_scaled_rotated_bitmap(
					ptr(),
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					scale.x, scale.y,
					angle,
					0
			);
		}

		void drawTintedScaledRotated(Color tint, Coord<float> centerSrc, Coord<float> centerDst, Vec2<float> scale, float angle) const {
			al_draw_tinted_scaled_rotated_bitmap(
					ptr(),
					tint,
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					scale.x, scale.y,
					angle,
					0
			);
		}

		void drawTintedScaledRotatedRegion(Rect<float> srcRegion, Color tint, Coord<float> centerSrc, Coord<float> centerDst, Vec2<float> scale, float angle) const {
			al_draw_tinted_scaled_rotated_bitmap_region(
					ptr(),
					srcRegion.a.x, srcRegion.a.y,
					srcRegion.width(), srcRegion.height(),
					tint,
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					scale.x, scale.y,
					angle,
					0
			);
		}
		
		/// @brief al_convert_mask_to_alpha()
		void convertMaskToAlpha(Color maskColor) {
			al_convert_mask_to_alpha(ptr(), maskColor);
		}

		/// @brief Sets all pixels of the bitmap to the given color.
		void clearToColor(Color color) {
			ScopedTargetBitmap tb(*this);
			al_clear_to_color(color);
		}

		/// @brief Equivalent to clearToColor(al::Black).
		void clear() {
			clearToColor(al::Black);
		}

		/// @brief al_save_bitmap()
		void saveToFile(const std::string& filename) const {
			al_save_bitmap(filename.c_str(), ptr());
		}

		/**
		 * @brief Clones the Bitmap. This method replaces the copy 
		 *  constructor to enforce being explicit about cloning.
		 * 
		 * @return A second identical Bitmap.
		 */
		[[nodiscard]] Bitmap clone() const {
			return Bitmap(al_clone_bitmap(ptr()));
		}

		BitmapLockedRegion lock(int format = ALLEGRO_PIXEL_FORMAT_ARGB_8888, int flags = ALLEGRO_LOCK_READWRITE);

		static int GetNewBitmapFlags() {
			return al_get_new_bitmap_flags();
		}
		static void SetNewBitmapFlags(int flags) {
			al_set_new_bitmap_flags(flags);
		}
	protected:
	};

	inline ScopedTargetBitmap::ScopedTargetBitmap(Bitmap& bmp)
		: ScopedTargetBitmap(bmp.ptr())
	{}

	/**
	 * @brief Provides a RAII-style mechanism for setting new bitmap flags.
	 */
	class ScopedNewBitmapFlags {
		int oldFlags;
	public:
		explicit ScopedNewBitmapFlags(int newFlags)
		{
			oldFlags = Bitmap::GetNewBitmapFlags();
			Bitmap::SetNewBitmapFlags(newFlags);
		}
		~ScopedNewBitmapFlags()
		{
			Bitmap::SetNewBitmapFlags(oldFlags);
		}
	};

	/**
	 * @brief Provides bitmap locking.
	 */
	class BitmapLockedRegion {
	public:
		BitmapLockedRegion(Bitmap& bmp, int format, int flags) {
			this->bmp = bmp.ptr();
			reg = al_lock_bitmap(this->bmp, format, flags);
			if(!reg) {
				throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
			}
		}

		BitmapLockedRegion(Bitmap& bmp, Rect<int> region, int format, int flags) {
			this->bmp = bmp.ptr();
			if(region.a.x < 0 || region.a.y < 0 || region.b.x >= bmp.width() || region.b.y >= bmp.height()) {
				throw BitmapLockError("Can't lock a region that extends past the bitmap");
			}
			reg = al_lock_bitmap_region(this->bmp, region.a.x, region.a.y, region.width(), region.height(), format, flags);
			if(!reg) {
				throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
			}
		}

		~BitmapLockedRegion() {
			al_unlock_bitmap(bmp);
		}
		
		BitmapLockedRegion(BitmapLockedRegion&) = delete;
		BitmapLockedRegion(BitmapLockedRegion&&) = delete; //TODO maybe moving should be allowed?
		BitmapLockedRegion& operator=(BitmapLockedRegion&) = delete;
		BitmapLockedRegion& operator=(BitmapLockedRegion&&) = delete;

		void* data() {
			return reg->data;
		}
		void* rawRowData(unsigned rowIndex) {
			return (uint8_t*)data() + ((int)rowIndex*reg->pitch);
		}

		[[nodiscard]] int getFormat() const {
			return reg->format;
		}
		[[nodiscard]] int getPitch() const {
			return reg->pitch;
		}
		[[nodiscard]] int getPixelSize() const {
			return reg->pixel_size;
		}

		template<typename T>
		T* rowData(unsigned rowIndex)
		{
			//TODO assert correct format
			return reinterpret_cast<T*>(rawRowData(rowIndex));
		}
		
	private:
		ALLEGRO_LOCKED_REGION* reg;
		ALLEGRO_BITMAP* bmp;
	};

	inline al::BitmapLockedRegion al::Bitmap::lock(int format, int flags)
	{
		return BitmapLockedRegion(*this, format, flags);
	}


#define AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC [[maybe_unused]] auto* unused_____ = ptr();

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
			useProjectionTransform(al::Transform::Orthographic({0,0}, {size()}, -1, 1));
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
		void setClippingRectangle(al::Rect<int> r) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_set_clipping_rectangle(r.a.x, r.a.y, r.width(), r.height());
		}
		[[nodiscard]] al::Rect<int> getClippingRectangle() const { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			int x,y,w,h;
			al_get_clipping_rectangle(&x, &y, &w, &h);
			return al::Rect<int>::XYWH(x,y,w,h);
		}
		void resetClippingRectangle() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_reset_clipping_rectangle();
		}

	private:
		[[nodiscard]] ALLEGRO_BITMAP* getPointer() const override
		{
			return al_get_target_bitmap();
		}
	};

#undef AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC

	inline CTargetBitmap TargetBitmap;
}


#endif /* INCLUDE_AXXEGRO_RESOURCES_BITMAP */
