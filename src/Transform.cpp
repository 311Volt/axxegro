#define AXXEGRO_TRUSTED

#include <axxegro/Transform.hpp>

#include <allegro5/allegro.h>

al::Transform::Transform()
{
	al_identity_transform(&t);
}

al::Transform::Transform(const ALLEGRO_TRANSFORM* src)
{
	al_copy_transform(&t, src);
}
		
void al::Transform::use() const
{
	al_use_transform(&t);
}
void al::Transform::useProjection() const
{
	al_use_projection_transform(&t);
}

al::Transform al::Transform::GetCurrent()
{
	return al::Transform(al_get_current_transform());
}
al::Transform al::Transform::GetCurrentProjection()
{
	return al::Transform(al_get_current_projection_transform());
}

al::Transform al::Transform::Eye()
{
	return Transform();
}
al::Transform al::Transform::Identity()
{
	return Transform();
}

al::Transform al::Transform::Build(float theta, const al::Vec2<float>& scale, const al::Vec2<float>& translation)
{
	Transform ret {Transform::NoInit()};
	al_build_transform(ret.ptr(), translation.x, translation.y, scale.x, scale.y, theta);
	return ret;
}
al::Transform al::Transform::Camera(const al::Vec3<float>& pos, const al::Vec3<float>& look, const al::Vec3<float>& up)
{
	Transform ret {Transform::NoInit()};
	al_build_camera_transform(
		ret.ptr(), 
		pos.x, pos.y, pos.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z
	);
	return ret;
}
al::Transform al::Transform::Orthographic(const al::Vec2<float>& leftTop, const al::Vec2<float> rightBottom, float near, float far)
{
	Transform ret;
	al_orthographic_transform(
		ret.ptr(),
		leftTop.x, leftTop.y, near,
		rightBottom.x, rightBottom.y, far
	);
	return ret;
}
al::Transform al::Transform::Perspective(const al::Vec2<float>& leftTop, const al::Vec2<float> rightBottom, float near, float far)
{
	Transform ret;
	al_perspective_transform(
		ret.ptr(),
		leftTop.x, leftTop.y, near,
		rightBottom.x, rightBottom.y, far
	);
	return ret;
}

al::Transform& al::Transform::invert()
{
	al_invert_transform(&t);
	return *this;
}
al::Transform& al::Transform::transpose()
{
	al_transpose_transform(&t);
	return *this;
}

al::Transform& al::Transform::translate(const al::Vec2<float>& v)
{
	al_translate_transform(&t, v.x, v.y);
	return *this;
}
al::Transform& al::Transform::scale(const al::Vec2<float>& scale)
{
	al_scale_transform(&t, scale.x, scale.y);
	return *this;
}
al::Transform& al::Transform::rotate(float theta)
{
	al_rotate_transform(&t, theta);
	return *this;
}

al::Transform& al::Transform::translate(const al::Vec3<float>& v)
{
	al_translate_transform_3d(&t, v.x, v.y, v.z);
	return *this;
}
al::Transform& al::Transform::scale(const al::Vec3<float>& scale)
{
	al_scale_transform_3d(&t, scale.x, scale.y, scale.z);
	return *this;
}
al::Transform& al::Transform::rotate(const al::Vec3<float>& center, float theta)
{
	al_rotate_transform_3d(&t, center.x, center.y, center.z, theta);
	return *this;
}

al::Transform& al::Transform::horizontalShear(float theta)
{
	al_horizontal_shear_transform(&t, theta);
	return *this;
}
al::Transform& al::Transform::verticalShear(float theta)
{
	al_vertical_shear_transform(&t, theta);
	return *this;
}

al::Coord2<float> al::Transform::transform(const al::Coord2<float>& v) const
{
	al::Coord2<float> ret(v);
	al_transform_coordinates(&t, &ret.x, &ret.y);
	return ret;
}
al::Coord3<float> al::Transform::transform(const al::Coord3<float>& v) const
{
	al::Coord3<float> ret(v);
	al_transform_coordinates_3d(&t, &ret.x, &ret.y, &ret.z);
	return ret;
}
al::Coord4<float> al::Transform::transform(const al::Coord4<float>& v) const
{
	al::Coord4<float> ret(v);
	al_transform_coordinates_4d(&t, &ret.x, &ret.y, &ret.z, &ret.w);
	return ret;
}

al::Coord3<float> al::Transform::transformProjective(const al::Coord3<float>& v) const
{
	al::Coord3<float> ret(v);
	al_transform_coordinates_3d_projective(&t, &ret.x, &ret.y, &ret.z);
	return ret;
}


al::Transform& al::Transform::compose(const al::Transform& other)
{
	al_compose_transform(&t, other.ptr());
	return *this;
}


bool al::Transform::checkInverse(float tolerance) const
{
	return al_check_inverse(&t, tolerance);
}

al::ScopedTransform::ScopedTransform(ALLEGRO_TRANSFORM* t)
	: originalTransform(al_get_current_transform())
{
	al_use_transform(t);
}

al::ScopedTransform::ScopedTransform(const al::Transform& t)
	: ScopedTransform(t.ptr())
{
	
}

al::ScopedTransform::~ScopedTransform()
{
	al_use_transform(originalTransform.ptr());
}