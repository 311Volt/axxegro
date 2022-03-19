#ifndef INCLUDE_AXXEGRO_RESOURCES_BITMAP
#define INCLUDE_AXXEGRO_RESOURCES_BITMAP

#include "Resource.hpp"
#include "ResourceHandle.hpp"

#include <allegro5/allegro.h>
#include <algorithm>
#include <string>

#include "../math/math.hpp"
#include "../Color.hpp"

/**
 * @file
 * An ALLEGRO_BITMAP wrapper.
 */

namespace al {

	class Bitmap: public Resource {
	public:
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

		/** 
		 *  @brief Constructs an object from a given pointer. It is important
		 *  to note that the dtor behaves the same (i.e. calls al_destroy_bitmap())
		 *  regardless of the ctor used to create the object. Using this ctor can be 
		 *  thought of as giving ownership of a bitmap to a new Bitmap object.
		 **/
		Bitmap(ALLEGRO_BITMAP* ptr);

		/**
		 * @brief Destroy the Bitmap object by calling al_destroy_bitmap() on ptr.
		 **/
		~Bitmap();

		/**
		 * @brief Returns the Allegro pointer. 
		 * 
		 * @return A pointer to the underlying Allegro5 bitmap structure. 
		 */
		ALLEGRO_BITMAP* alPtr();

	#ifdef AXXEGRO_TRUSTED
		/**
		 * @brief Returns the Allegro pointer and expects the user to not change
		 * the bitmap. This method should not be available to the user - axxegro
		 * should aim to make every non-bitmap-chaging operation in allegro
		 * possible to do with a const Bitmap - if it doesn't, something's missing
		 * 
		 * @return A pointer to the underlying Allegro5 bitmap structure. 
		 */
		ALLEGRO_BITMAP* alPtr() const {return ptr;}
	#endif

		/// @return The width of the bitmap in pixels.
		int width() const;

		/// @return The height of the bitmap in pixels.
		int height() const;

		/// @return The dimensions (width x height) of the bitmap in pixels.
		Vec2 size() const;

		/**
		 * @return A rectangle spanning the entire bitmap, with the top left corner at (0, 0)
		 * and the bottom left corner at (width, height). Useful for the drawScaled() family
		 * of methods.
		 **/
		Rect rect() const;

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
		ALLEGRO_BITMAP* ptr;
	};

	class BitmapHandleImgFile: public ResourceHandle<Bitmap> {
	public:
		BitmapHandleImgFile(const std::string& filename);
		virtual void load() override;
	private:
		std::string filename;
	};

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
			: ScopedTargetBitmap(bmp.alPtr()) 
		{}

		~ScopedTargetBitmap()
		{
			al_set_target_bitmap(oldTarget);
		}
	};

	class BitmapLockedRegion {
	public:
		BitmapLockedRegion(Bitmap& bmp);
		//TODO
	};
}


#endif /* INCLUDE_AXXEGRO_RESOURCES_BITMAP */