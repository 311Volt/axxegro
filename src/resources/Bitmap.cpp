#define AXXEGRO_TRUSTED

#include <axxegro/resources/Bitmap.hpp>
#include <axxegro/Color.hpp>

#include <axxegro/Exception.hpp>

#include <stdexcept>
#include <format>
#include <iostream>

al::CTargetBitmap al::TargetBitmap;

al::Bitmap::Bitmap(int w, int h)
	: Resource(al_create_bitmap(w,h))
{
	if(!ptr()) {
		throw ResourceLoadError("Error while creating a {}x{} bitmap", w, h);
	}
}
al::Bitmap::Bitmap(int w, int h, al::Color color)
	: Bitmap(w,h)
{
	clearToColor(color);
}
al::Bitmap::Bitmap(const std::string& filename)
	: Resource(al_load_bitmap(filename.c_str()))
{	
	if(!ptr()) {
		throw ResourceLoadError("Cannot load bitmap from {} - file missing, corrupted or in an unsupported format", filename);
	}
}


int al::Bitmap::width() const
{
	return al_get_bitmap_width(ptr());
}
int al::Bitmap::height() const
{
	return al_get_bitmap_height(ptr());
}
al::Vec2<int> al::Bitmap::size() const
{
	return {width(), height()};
}
al::Rect<int> al::Bitmap::rect() const
{
	return {{0, 0}, size()};
}

void al::Bitmap::draw(al::Coord<float> p0) const
{
	al_draw_bitmap(ptr(), p0.x, p0.y, 0);
}
void al::Bitmap::drawTinted(al::Coord<float> p0, al::Color tint) const
{
	al_draw_tinted_bitmap(ptr(), tint, p0.x, p0.y, 0);
}
void al::Bitmap::drawRegion(al::Rect<float> srcRegion, al::Coord<float> dst) const
{
	al_draw_bitmap_region(
		ptr(), 
		srcRegion.a.x, srcRegion.a.y, 
		srcRegion.width(), srcRegion.height(),
		dst.x, dst.y,
		0
	);
}
void al::Bitmap::drawScaled(al::Rect<float> srcRect, al::Rect<float> dstRect) const
{
	al_draw_scaled_bitmap(
		ptr(), 
		srcRect.a.x, srcRect.a.y,
		srcRect.width(), srcRect.height(),
		dstRect.a.x, dstRect.a.y,
		dstRect.width(), dstRect.height(),
		0
	);
}
void al::Bitmap::drawRotated(al::Coord<float> centerSrc, al::Coord<float> centerDst, float angle) const
{
	al_draw_rotated_bitmap(
		ptr(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaled(al::Color tint, al::Rect<float> srcRect, al::Rect<float> dstRect) const
{
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
void al::Bitmap::drawTintedRegion(al::Rect<float> srcRegion, al::Coord<float> dst, al::Color tint) const
{
	al_draw_tinted_bitmap_region(
		ptr(),
		tint,
		srcRegion.a.x, srcRegion.a.y,
		srcRegion.width(), srcRegion.height(),
		dst.x, dst.y,
		0
	);
}
void al::Bitmap::drawTintedRotated(al::Color tint, al::Coord<float> centerSrc, al::Coord<float> centerDst, float angle) const
{
	al_draw_tinted_rotated_bitmap(
		ptr(),
		tint,
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		angle,
		0
	);
}
void al::Bitmap::drawScaledRotated(al::Coord<float> centerSrc, al::Coord<float> centerDst, al::Vec2<float> scale, float angle) const
{
	al_draw_scaled_rotated_bitmap(
		ptr(),
		centerSrc.x, centerSrc.y,
		centerDst.x, centerDst.y,
		scale.x, scale.y,
		angle,
		0
	);
}
void al::Bitmap::drawTintedScaledRotated(al::Color tint, al::Coord<float> centerSrc, al::Coord<float> centerDst, al::Vec2<float> scale, float angle) const
{
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
void al::Bitmap::drawTintedScaledRotatedRegion(al::Rect<float> srcRegion, al::Color tint, al::Coord<float> centerSrc, al::Coord<float> centerDst, al::Vec2<float> scale, float angle) const
{
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

void al::Bitmap::convertMaskToAlpha(al::Color maskColor)
{
	al_convert_mask_to_alpha(ptr(), maskColor);
}
void al::Bitmap::clearToColor(al::Color color)
{
	ScopedTargetBitmap tb(*this);
	al_clear_to_color(color);
}

void al::Bitmap::saveToFile(const std::string& filename) const
{
	al_save_bitmap(filename.c_str(), ptr());
}
al::Bitmap al::Bitmap::clone() const
{
	return Bitmap(al_clone_bitmap(ptr()));
}

int al::Bitmap::GetNewBitmapFlags()
{
	return al_get_new_bitmap_flags();
}

void al::Bitmap::SetNewBitmapFlags(int flags)
{
	al_set_new_bitmap_flags(flags);
}

void al::Bitmap::clear()
{
	clearToColor(al::Black);
}

al::BitmapLockedRegion al::Bitmap::lock(int format, int flags)
{
	return BitmapLockedRegion(*this, format, flags);
}

al::BitmapLockedRegion::BitmapLockedRegion(Bitmap& bmp, int format, int flags)
{
	this->bmp = bmp.ptr();
	reg = al_lock_bitmap(this->bmp, format, flags);
	if(!reg) {
		throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
	}
}

al::BitmapLockedRegion::BitmapLockedRegion(Bitmap& bmp, Rect<int> region, int format, int flags)
{
	this->bmp = bmp.ptr();
	if(region.a.x < 0 || region.a.y < 0 || region.b.x >= bmp.width() || region.b.y >= bmp.height()) {
		throw BitmapLockError("Can't lock a region that extends past the bitmap");
	}
	reg = al_lock_bitmap_region(this->bmp, region.a.x, region.a.y, region.width(), region.height(), format, flags);
	if(!reg) {
		throw BitmapLockError("Error while locking bitmap. Make sure the bitmap isn't locked already and that you have specified a correct format.");
	}
}

al::BitmapLockedRegion::~BitmapLockedRegion()
{
	al_unlock_bitmap(bmp);
}

void* al::BitmapLockedRegion::data()
{
	return reg->data;
}
void* al::BitmapLockedRegion::rawRowData(unsigned rowIndex)
{
	return (uint8_t*)data() + ((int)rowIndex*reg->pitch);
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

void al::CTargetBitmap::clearToColor(al::Color color)
{
	al_clear_to_color(color);
}

void al::CTargetBitmap::clearDepthBuffer(float x)
{
	al_clear_depth_buffer(x);
}
al::Rect<int> al::CTargetBitmap::getClippingRectangle()
{
	int x,y,w,h;
	al_get_clipping_rectangle(&x, &y, &w, &h);
	return al::Rect<int>::XYWH(x,y,w,h);
}
void al::CTargetBitmap::setClippingRectangle(al::Rect<int> r)
{
	al_set_clipping_rectangle(r.a.x, r.a.y, r.width(), r.height());
}
void al::CTargetBitmap::resetClippingRectangle()
{
	al_reset_clipping_rectangle();
}

void al::CTargetBitmap::setRenderState(ALLEGRO_RENDER_STATE state, int value)
{
	al_set_render_state(state, value);
}
void al::CTargetBitmap::setAlphaTest(bool value)
{
	setRenderState(ALLEGRO_ALPHA_TEST, value);
}
void al::CTargetBitmap::setAlphaFunction(ALLEGRO_RENDER_FUNCTION value)
{
	setRenderState(ALLEGRO_ALPHA_FUNCTION, value);
}
void al::CTargetBitmap::setAlphaTestValue(uint8_t value)
{
	setRenderState(ALLEGRO_ALPHA_TEST_VALUE, value);
}
void al::CTargetBitmap::setWriteMaskFlags(ALLEGRO_WRITE_MASK_FLAGS value)
{
	setRenderState(ALLEGRO_WRITE_MASK, value);
}
void al::CTargetBitmap::setDepthTest(bool value)
{
	setRenderState(ALLEGRO_DEPTH_TEST, value);
}
void al::CTargetBitmap::setDepthFunction(ALLEGRO_RENDER_FUNCTION value)
{
	setRenderState(ALLEGRO_DEPTH_FUNCTION, value);
}


void al::CTargetBitmap::useTransform(const al::Transform& transform)
{
	al_use_transform(&transform);
}
void al::CTargetBitmap::useProjectionTransform(const al::Transform& transform)
{
	al_use_projection_transform(&transform);
}

void al::CTargetBitmap::resetTransform()
{
	useTransform(al::Transform::Identity());
}
void al::CTargetBitmap::resetProjection()
{
	useProjectionTransform(al::Transform::Orthographic({0,0}, {size()}, -1, 1));
}

al::Transform al::CTargetBitmap::currentTransform()
{
	return al_get_current_transform();
}
al::Transform al::CTargetBitmap::currentInverseTransform()
{
	return al_get_current_inverse_transform();
}
al::Transform al::CTargetBitmap::currentProjectionTransform()
{
	return al_get_current_projection_transform();
}
