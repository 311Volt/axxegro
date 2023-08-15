#ifndef INCLUDE_AXXEGRO_RESOURCES_BITMAP
#define INCLUDE_AXXEGRO_RESOURCES_BITMAP

#include "../../common.hpp"
#include "Color.hpp"
#include "Pixel.hpp"

#include "axxegro/com/Exception.hpp"
#include "axxegro/com/Resource.hpp"
#include <memory>
#include <span>

/**
 * @file
 * An ALLEGRO_BITMAP wrapper plus utilities.
 */

namespace al {
	AXXEGRO_DEFINE_DELETER(ALLEGRO_BITMAP, al_destroy_bitmap);

	class BaseLockedBitmapRegion;
	class Bitmap;
	class SubBitmap;

	template<typename T, bool TPReadOnly>
	class LockedBitmapRegion;

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
		
		ScopedTargetBitmap(const ScopedTargetBitmap&) = delete;
		ScopedTargetBitmap& operator=(const ScopedTargetBitmap&) = delete;
		ScopedTargetBitmap(ScopedTargetBitmap&&) = delete;
		ScopedTargetBitmap& operator=(ScopedTargetBitmap&&) = delete;
		
	};
	/**
	 * @brief Wraps around ALLEGRO_BITMAP.
	 */
	class Bitmap:
			RequiresInitializables<CoreAllegro>,
			public Resource<ALLEGRO_BITMAP> {
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
		[[nodiscard]] RectI rect() const {
			return {{0, 0}, size()};
		}

		/* note: I left out the flags parameter. al_draw_xxx_bitmap is
		   always called with flags=0. use transforms/texcoords for flipping. */

		/// @brief Corresponds to al_draw_bitmap(). Refer to Allegro5 documentation for more info on this and other drawX() methods.
		void draw(Vec2f p0) const {
			al_draw_bitmap(ptr(), p0.x, p0.y, 0);
		}

		void drawTinted(Vec2f p0, Color tint) const {
			al_draw_tinted_bitmap(ptr(), tint, p0.x, p0.y, 0);
		}

		void drawRegion(RectF srcRegion, Vec2f dst) const {
			al_draw_bitmap_region(
					ptr(),
					srcRegion.a.x, srcRegion.a.y,
					srcRegion.width(), srcRegion.height(),
					dst.x, dst.y,
					0
			);
		}

		void drawScaled(RectF srcRect, RectF dstRect) const {
			al_draw_scaled_bitmap(
					ptr(),
					srcRect.a.x, srcRect.a.y,
					srcRect.width(), srcRect.height(),
					dstRect.a.x, dstRect.a.y,
					dstRect.width(), dstRect.height(),
					0
			);
		}
		void drawRotated(Vec2f centerSrc, Vec2f centerDst, float angle) const {
			al_draw_rotated_bitmap(
					ptr(),
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					angle,
					0
			);
		}

		void drawTintedScaled(Color tint, RectF srcRect, RectF dstRect) const {
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

		void drawTintedRegion(RectF srcRegion, Vec2f dst, Color tint) const {
			al_draw_tinted_bitmap_region(
					ptr(),
					tint,
					srcRegion.a.x, srcRegion.a.y,
					srcRegion.width(), srcRegion.height(),
					dst.x, dst.y,
					0
			);
		}

		void drawTintedRotated(Color tint, Vec2f centerSrc, Vec2f centerDst, float angle) const {
			al_draw_tinted_rotated_bitmap(
					ptr(),
					tint,
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					angle,
					0
			);
		}

		void drawScaledRotated(Vec2f centerSrc, Vec2f centerDst, Vec2f scale, float angle) const {
			al_draw_scaled_rotated_bitmap(
					ptr(),
					centerSrc.x, centerSrc.y,
					centerDst.x, centerDst.y,
					scale.x, scale.y,
					angle,
					0
			);
		}

		void drawTintedScaledRotated(Color tint, Vec2f centerSrc, Vec2f centerDst, Vec2f scale, float angle) const {
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

		void drawTintedScaledRotatedRegion(RectF srcRegion, Color tint, Vec2f centerSrc, Vec2f centerDst, Vec2f scale, float angle) const {
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
		[[nodiscard]] auto clone() const {
			return Bitmap(al_clone_bitmap(ptr()));
		}

		[[nodiscard]] SubBitmap createSubBitmap(RectI rect);

		BaseLockedBitmapRegion lockDynamic(int format = ALLEGRO_PIXEL_FORMAT_ARGB_8888, int flags = ALLEGRO_LOCK_READWRITE);

		template<FormattedPixelType T>
		LockedBitmapRegion<T, false> lock(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, T::PixelFormat, false>(region, ALLEGRO_LOCK_READWRITE);
		}

		template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat>
		LockedBitmapRegion<T, false> lock(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, TPPixelFormat, false>(region, ALLEGRO_LOCK_READWRITE);
		}

		template<FormattedPixelType T>
		LockedBitmapRegion<T, true> lockReadOnly(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, T::PixelFormat, true>(region, ALLEGRO_LOCK_READONLY);
		}

		template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat>
		LockedBitmapRegion<T, true> lockReadOnly(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, TPPixelFormat, true>(region, ALLEGRO_LOCK_READONLY);
		}

		template<FormattedPixelType T>
		LockedBitmapRegion<T, false> lockWriteOnly(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, T::PixelFormat, false>(region, ALLEGRO_LOCK_WRITEONLY);
		}

		template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat>
		LockedBitmapRegion<T, false> lockWriteOnly(RectI region = {0,0,-1,-1}) {
			return lockImpl<T, TPPixelFormat, false>(region, ALLEGRO_LOCK_WRITEONLY);
		}



		static int GetNewBitmapFlags() {
			return al_get_new_bitmap_flags();
		}
		static void SetNewBitmapFlags(int flags) {
			al_set_new_bitmap_flags(flags);
		}

		friend class Video;
	protected:

		template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat, bool TPReadOnly>
		requires CanRepresentPixelFormat<T, TPPixelFormat>
		LockedBitmapRegion<T, TPReadOnly> lockImpl(RectI region, int flags);
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


	class SubBitmap: public Bitmap {
	public:
		[[nodiscard]] Vec2i parentXY() const {
			return {al_get_bitmap_x(ptr()), al_get_bitmap_y(ptr())};
		}
		Bitmap& getParent() {
			return parent;
		}

		bool reparent(Bitmap& bitmap, RectI rect = {0, 0, -1, -1}) {
			if(rect.width() < 0) {
				rect = mrect;
			}
			al_reparent_bitmap(bitmap.ptr(), ptr(), rect.a.x, rect.a.y, rect.width(), rect.height());
			return true;
		}

		SubBitmap(SubBitmap&&) = delete;
		SubBitmap& operator=(SubBitmap&&) = delete;

		friend class Bitmap;

	private:
		SubBitmap(Bitmap& parent, const RectI rect)
			: Bitmap(al_create_sub_bitmap(parent.ptr(), rect.a.x, rect.a.y, rect.width(), rect.height())),
			  mrect(rect),
			  parent(parent.ptr(), ResourceModel::NonOwning)
		{
			if(!ptr()) {
				throw ResourceLoadError(
					"Error while creating a sub-bitmap of a %dx%d bitmap with position (%d, %d) and size %dx%d",
					parent.width(), parent.height(),
					rect.a.x, rect.a.y,
					rect.width(), rect.height()
				);
			}
		}

		RectI mrect;
		Bitmap parent;
	};

	inline SubBitmap Bitmap::createSubBitmap(RectI rect) {
		return SubBitmap(*this, rect);
	}


	/**
	 * @brief Provides bitmap locking.
	 */
	class BaseLockedBitmapRegion {
	public:
		BaseLockedBitmapRegion(Bitmap& bmp, int format, int flags) {
			this->bmp = bmp.ptr();
			reg = al_lock_bitmap(this->bmp, format, flags);
			if(!reg) {
				throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
			}
			regionSize = bmp.size();
		}

		BaseLockedBitmapRegion(Bitmap& bmp, RectI region, int format, int flags) {
			this->bmp = bmp.ptr();
			if(region.a.x < 0 || region.a.y < 0 || region.b.x > bmp.width() || region.b.y > bmp.height()) {
				throw BitmapLockError("Can't lock a region that extends past the bitmap");
			}
			reg = al_lock_bitmap_region(this->bmp, region.a.x, region.a.y, region.width(), region.height(), format, flags);
			if(!reg) {
				throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
			}
			regionSize = region.size();
		}

		~BaseLockedBitmapRegion() {
			al_unlock_bitmap(bmp);
		}
		
		BaseLockedBitmapRegion(BaseLockedBitmapRegion&) = delete;
		BaseLockedBitmapRegion& operator=(BaseLockedBitmapRegion&) = delete;
		BaseLockedBitmapRegion(BaseLockedBitmapRegion&&) = delete;
		BaseLockedBitmapRegion& operator=(BaseLockedBitmapRegion&&) = delete;

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
		[[nodiscard]] Vec2i size() const {
			return regionSize;
		}
		[[nodiscard]] int width() const {
			return regionSize.x;
		}
		[[nodiscard]] int height() const {
			return regionSize.y;
		}

		template<typename T>
		T* rowData(unsigned rowIndex)
		{
			//TODO assert correct format
			return reinterpret_cast<T*>(rawRowData(rowIndex));
		}

	protected:
		al::Vec2i regionSize;
		ALLEGRO_LOCKED_REGION* reg;
		ALLEGRO_BITMAP* bmp;
	};

	inline al::BaseLockedBitmapRegion al::Bitmap::lockDynamic(int format, int flags)
	{
		return BaseLockedBitmapRegion(*this, format, flags);
	}


	template<typename PixelT, bool TPReadOnly>
	class LockedBitmapRegion: public BaseLockedBitmapRegion {
	public:
		using RowSpanT = std::conditional_t<
		    TPReadOnly,
			std::span<const PixelT>,
			std::span<PixelT>
		>;

		[[nodiscard]] RowSpanT row(unsigned rowIdx) {
			return RowSpanT(
				reinterpret_cast<PixelT*>(rawRowData(rowIdx)),
				reinterpret_cast<PixelT*>(rawRowData(rowIdx)) + width()
			);
		}

	private:
		friend class Bitmap;
		LockedBitmapRegion(Bitmap& bmp, RectI region, int format, int flags)
			: BaseLockedBitmapRegion(bmp, region, format, flags)
		{

			unsigned expected = sizeof(PixelT);
			unsigned actual = getPixelSize();
			if(expected != actual) {
				throw BitmapLockError(
					"expected %u-byte pixels, got %u-byte pixels (this should never happen)",
					expected, actual
				);
			}
		}
	};

	template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat, bool TPReadOnly>
	requires CanRepresentPixelFormat<T, TPPixelFormat>
	LockedBitmapRegion<T, TPReadOnly> Bitmap::lockImpl(RectI region, int flags) {
		if(region.width() <= 0) {
			region = this->rect();
		}
		return LockedBitmapRegion<T, TPReadOnly>(*this, region, TPPixelFormat, flags);
	}
}


#endif /* INCLUDE_AXXEGRO_RESOURCES_BITMAP */
