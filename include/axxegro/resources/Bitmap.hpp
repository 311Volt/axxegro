#ifndef INCLUDE_AXXEGRO_RESOURCES_BITMAP
#define INCLUDE_AXXEGRO_RESOURCES_BITMAP

#include "Resource.hpp"
#include <allegro5/allegro.h>

#include <stdexcept>
#include <algorithm>
#include <string>
#include <memory>

#include "../math/math.hpp"
#include "../Color.hpp"

/**
 * @file
 * An ALLEGRO_BITMAP wrapper plus utilities.
 */

namespace al {

	class BitmapLockError: public std::runtime_error {using std::runtime_error::runtime_error;};

	///@brief ALLEGRO_BITMAP deleter for C++ smart pointers
	class BitmapDeleter {
	public:
		void operator()(ALLEGRO_BITMAP* ptr){al_destroy_bitmap(ptr);}
	};

	/**
	 * @brief Wraps around ALLEGRO_BITMAP.
	 */
	class Bitmap: public Resource<ALLEGRO_BITMAP, BitmapDeleter> {
	public:
		using Resource::Resource;
		Bitmap() = delete;
		
		/** @brief Creates a bitmap.
		 * @param w Width in pixels.
		 * @param h Height in pixels. 
		 **/
		Bitmap(int w, int h);

		/**
		 * @brief Creates a bitmap that is pre-cleared to a given color.
		 */
		Bitmap(int w, int h, Color color);

		/** 
		 * @brief Loads a bitmap from a file. 
		 * @param filename Path to the file with the image.
		 **/
		Bitmap(const std::string& filename);

		/// @return The width of the bitmap in pixels.
		int getWidth() const;

		/// @return The height of the bitmap in pixels.
		int getHeight() const;

		/// @return The dimensions (width x height) of the bitmap in pixels.
		Vec2 getSize() const;

		/**
		 * @return A rectangle spanning the entire bitmap, with the top left corner at (0, 0)
		 * and the bottom left corner at (width, height). Useful for the drawScaled() family
		 * of methods.
		 **/
		Rect getRect() const;

		/* note: I left out the flags parameter. al_draw_xxx_bitmap is
		   always called with flags=0. use transforms/texcoords for flipping. */

		/// @brief Corresponds to al_draw_bitmap(). Refer to Allegro5 documentation for more info on this and other drawX() methods.
		void draw(Point p0) const;
		void drawTinted(Point p0, Color tint) const;
		void drawRegion(Rect srcRegion, Point dst) const;
		void drawScaled(Rect srcRect, Rect dstRect) const;
		void drawRotated(Point centerSrc, Point centerDst, float angle) const;
		void drawTintedScaled(Color tint, Rect srcRect, Rect dstRect) const;
		void drawTintedRegion(Rect srcRegion, Point dst, Color tint) const;
		void drawTintedRotated(Color tint, Point centerSrc, Point centerDst, float angle) const;
		void drawScaledRotated(Point centerSrc, Point centerDst, Vec2 scale, float angle) const;
		void drawTintedScaledRotated(Color tint, Point centerSrc, Point centerDst, Vec2 scale, float angle) const;
		void drawTintedScaledRotatedRegion(Rect srcRegion, Color tint, Point centerSrc, Point centerDst, Vec2 scale, float angle) const;
		
		/// @brief al_convert_mask_to_alpha()
		void convertMaskToAlpha(Color maskColor);

		/// @brief Sets all pixels of the bitmap to the given color.
		void clearToColor(Color color);

		/// @brief al_save_bitmap()
		void saveToFile(const std::string& filename) const;

		/**
		 * @brief Clones the Bitmap. This method replaces the copy 
		 *  constructor to enforce being explicit about cloning.
		 * 
		 * @return A second identical Bitmap.
		 */
		Bitmap clone() const;

	protected:
	};

	/**
	 * @brief Provides a RAII-style mechanism for setting target bitmaps.
	 */
	class ScopedTargetBitmap {
	private:
		ALLEGRO_BITMAP* oldTarget;
	public:
		ScopedTargetBitmap(ALLEGRO_BITMAP* bmp)
		{
			oldTarget = al_get_target_bitmap();
			al_set_target_bitmap(bmp);
		}
		ScopedTargetBitmap(Bitmap& bmp)
			: ScopedTargetBitmap(bmp.ptr()) 
		{}

		~ScopedTargetBitmap()
		{
			al_set_target_bitmap(oldTarget);
		}
	};

	/**
	 * @brief Provides bitmap locking.
	 */
	class BitmapLockedRegion {
	public:
		BitmapLockedRegion(Bitmap& bmp, int format, int flags);
		BitmapLockedRegion(Bitmap& bmp, Rect region, int format, int flags);
		~BitmapLockedRegion();
		
		BitmapLockedRegion(BitmapLockedRegion&) = delete;
		BitmapLockedRegion(BitmapLockedRegion&&) = delete;
		BitmapLockedRegion& operator=(BitmapLockedRegion&) = delete;
		BitmapLockedRegion& operator=(BitmapLockedRegion&&) = delete;

		uint8_t* data();
		uint8_t* rowData(unsigned rowIndex);
		int getFormat();
		int getPitch();
		int getPixelSize();
	private:
		ALLEGRO_LOCKED_REGION* reg;
		ALLEGRO_BITMAP* bmp;
	};

	class CTargetBitmap: public Bitmap {
	public:
		CTargetBitmap() : Bitmap(nullptr) {}
	private:
		virtual ALLEGRO_BITMAP* getPointer() const override
		{
			return al_get_target_bitmap();
		}
	};
}


#endif /* INCLUDE_AXXEGRO_RESOURCES_BITMAP */
