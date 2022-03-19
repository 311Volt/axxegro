#include <axxegro/resources/Bitmap.hpp>
#include <axxegro/Color.hpp>

#include <stdexcept>

al::Bitmap::Bitmap(int w, int h)
{
	ptr = al_create_bitmap(w, h);
}
al::Bitmap::Bitmap(int w, int h, al::Color color)
	: Bitmap(w,h)
{
	clearToColor(color);
}
al::Bitmap::Bitmap(const std::string& filename)
{
	ptr = al_load_bitmap(filename.c_str());
	if(!ptr) {
		throw std::runtime_error("cannot load bitmap "+filename);
	}
}
al::Bitmap::Bitmap(ALLEGRO_BITMAP* ptr)
{
	this->ptr = ptr;
}
al::Bitmap::~Bitmap()
{
	al_destroy_bitmap(ptr);
}
ALLEGRO_BITMAP* al::Bitmap::alPtr()
{
	return ptr;
}
int al::Bitmap::width() const
{
	return al_get_bitmap_width(ptr);
}
int al::Bitmap::height() const
{
	return al_get_bitmap_height(ptr);
}
al::Vec2 al::Bitmap::size() const
{
	return {width(), height()};
}
al::Rect al::Bitmap::rect() const
{
	return {{0, 0}, size()};
}

void al::Bitmap::draw(al::Point p0) const
{
	al_draw_bitmap(ptr, p0.x, p0.y, 0);
}
void al::Bitmap::drawTinted(al::Point p0, al::Color tint) const
{
	al_draw_tinted_bitmap(ptr, tint.get(), p0.x, p0.y, 0);
}
void al::Bitmap::drawRegion(al::Rect srcRegion, al::Point dst) const
{
	al_draw_bitmap_region(
		ptr, 
		srcRegion.a.x, srcRegion.a.y, 
		srcRegion.getWidth(), srcRegion.getHeight(),
		dst.x, dst.y,
		0
	);
}
void al::Bitmap::drawScaled(al::Rect srcRect, al::Rect dstRect) const
{
	al_draw_scaled_bitmap(
		ptr, 
		srcRect.a.x, srcRect.a.y,
		srcRect.getWidth(), srcRect.getHeight(),
		dstRect.a.x, dstRect.a.y,
		dstRect.getWidth(), dstRect.getHeight(),
		0
	);
}
void al::Bitmap::drawRotated(al::Point centerSrc, al::Point centerDst, float angle) const
{
	al_draw_rotated_bitmap(
		ptr,
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaled(al::Color tint, al::Rect srcRect, al::Rect dstRect) const
{
	al_draw_tinted_scaled_bitmap(
		ptr,
		tint.get(),
		srcRect.a.x, srcRect.a.y,
		srcRect.getWidth(), srcRect.getHeight(),
		dstRect.a.x, dstRect.a.y,
		dstRect.getWidth(), dstRect.getHeight(),
		0
	);
}
void al::Bitmap::drawTintedRegion(al::Rect srcRegion, al::Point dst, al::Color tint) const
{
	al_draw_tinted_bitmap_region(
		ptr,
		tint.get(),
		srcRegion.a.x, srcRegion.a.y,
		srcRegion.getWidth(), srcRegion.getHeight(),
		dst.x, dst.y,
		0
	);
}
void al::Bitmap::drawTintedRotated(al::Color tint, al::Point centerSrc, al::Point centerDst, float angle) const
{
	al_draw_tinted_rotated_bitmap(
		ptr,
		tint.get(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		angle,
		0
	);
}
void al::Bitmap::drawScaledRotated(al::Point centerSrc, al::Point centerDst, Vec2 scale, float angle) const
{
	al_draw_scaled_rotated_bitmap(
		ptr,
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		scale.x, scale.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaledRotated(al::Color tint, al::Point centerSrc, al::Point centerDst, Vec2 scale, float angle) const
{
	al_draw_tinted_scaled_rotated_bitmap(
		ptr,
		tint.get(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		scale.x, scale.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaledRotatedRegion(al::Rect srcRegion, al::Color tint, al::Point centerSrc, al::Point centerDst, Vec2 scale, float angle) const
{
	al_draw_tinted_scaled_rotated_bitmap_region(
		ptr,
		srcRegion.a.x, srcRegion.a.y,
		srcRegion.getWidth(), srcRegion.getHeight(),
		tint.get(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		scale.x, scale.y,
		angle,
		0
	);
}

void al::Bitmap::convertMaskToAlpha(al::Color maskColor)
{
	al_convert_mask_to_alpha(ptr, maskColor.get());
}
void al::Bitmap::clearToColor(al::Color color)
{
	ScopedTargetBitmap tb(ptr);
	al_clear_to_color(color.get());
}

void al::Bitmap::saveToFile(const std::string& filename) const
{
	al_save_bitmap(filename.c_str(), ptr);
}
al::Bitmap al::Bitmap::clone() const
{
	return Bitmap(al_clone_bitmap(ptr));
}

al::BitmapHandleImgFile::BitmapHandleImgFile(const std::string& filename)
	: filename(filename)
{
	
}

void al::BitmapHandleImgFile::load()
{
	resource = std::make_unique<Bitmap>(filename);
}
