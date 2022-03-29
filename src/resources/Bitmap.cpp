#include <axxegro/resources/Bitmap.hpp>
#include <axxegro/Color.hpp>

#include <stdexcept>

#include <fmt/format.h>

al::Bitmap::Bitmap(int w, int h)
{
	bmp = decltype(bmp)(al_create_bitmap(w, h));
}
al::Bitmap::Bitmap(int w, int h, al::Color color)
	: Bitmap(w,h)
{
	clearToColor(color);
}
al::Bitmap::Bitmap(const std::string& filename)
{
	bmp = decltype(bmp)(al_load_bitmap(filename.c_str()));
	if(!bmp) {
		throw ResourceLoadError(fmt::format(
			"Cannot load bitmap from \"{}\" - file missing, corrupted or invalid",
			filename
		));
	}
}
al::Bitmap::Bitmap(ALLEGRO_BITMAP* ptr)
{
	bmp = decltype(bmp)(ptr);
}

ALLEGRO_BITMAP* al::Bitmap::alPtr()
{
	return bmp.get();
}
int al::Bitmap::getWidth() const
{
	return al_get_bitmap_width(bmp.get());
}
int al::Bitmap::getHeight() const
{
	return al_get_bitmap_height(bmp.get());
}
al::Vec2 al::Bitmap::getSize() const
{
	return {getWidth(), getHeight()};
}
al::Rect al::Bitmap::getRect() const
{
	return {{0, 0}, getSize()};
}

void al::Bitmap::draw(al::Point p0) const
{
	al_draw_bitmap(bmp.get(), p0.x, p0.y, 0);
}
void al::Bitmap::drawTinted(al::Point p0, al::Color tint) const
{
	al_draw_tinted_bitmap(bmp.get(), tint.get(), p0.x, p0.y, 0);
}
void al::Bitmap::drawRegion(al::Rect srcRegion, al::Point dst) const
{
	al_draw_bitmap_region(
		bmp.get(), 
		srcRegion.a.x, srcRegion.a.y, 
		srcRegion.getWidth(), srcRegion.getHeight(),
		dst.x, dst.y,
		0
	);
}
void al::Bitmap::drawScaled(al::Rect srcRect, al::Rect dstRect) const
{
	al_draw_scaled_bitmap(
		bmp.get(), 
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
		bmp.get(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaled(al::Color tint, al::Rect srcRect, al::Rect dstRect) const
{
	al_draw_tinted_scaled_bitmap(
		bmp.get(),
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
		bmp.get(),
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
		bmp.get(),
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
		bmp.get(),
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
		bmp.get(),
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
		bmp.get(),
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
	al_convert_mask_to_alpha(bmp.get(), maskColor.get());
}
void al::Bitmap::clearToColor(al::Color color)
{
	ScopedTargetBitmap tb(*this);
	al_clear_to_color(color.get());
}

void al::Bitmap::saveToFile(const std::string& filename) const
{
	al_save_bitmap(filename.c_str(), bmp.get());
}
al::Bitmap al::Bitmap::clone() const
{
	return Bitmap(al_clone_bitmap(bmp.get()));
}

al::BitmapLockedRegion::BitmapLockedRegion(Bitmap& bmp, int format, int flags)
{
	this->bmp = bmp.alPtr();
	reg = al_lock_bitmap(this->bmp, format, flags);
	if(!reg) {
		throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
	}
}

al::BitmapLockedRegion::BitmapLockedRegion(Bitmap& bmp, Rect region, int format, int flags)
{
	this->bmp = bmp.alPtr();
	if(region.a.x < 0 || region.a.y < 0 || region.b.x >= bmp.getWidth() || region.b.y >= bmp.getHeight()) {
		throw BitmapLockError("Can't lock a region that extends past the bitmap");
	}
	reg = al_lock_bitmap_region(this->bmp, region.a.x, region.a.y, region.getWidth(), region.getHeight(), format, flags);
	if(!reg) {
		throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
	}
}

al::BitmapLockedRegion::~BitmapLockedRegion()
{
	al_unlock_bitmap(bmp);
}

uint8_t* al::BitmapLockedRegion::data()
{
	return (uint8_t*)reg->data;
}
uint8_t* al::BitmapLockedRegion::rowData(unsigned rowIndex)
{
	return data() + ((int)rowIndex*reg->pitch);
}
int al::BitmapLockedRegion::getFormat()
{
	return reg->format;
}
int al::BitmapLockedRegion::getPitch()
{
	return reg->pitch;
}
int al::BitmapLockedRegion::getPixelSize()
{
	return reg->pixel_size;
}