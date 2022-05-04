
#define AXXEGRO_TRUSTED

#include <axxegro/Transform.hpp>

#include <allegro5/allegro.h>

#include <cmath>
#include <stdexcept>
#include <fmt/format.h>

al::Transform::Transform()
{
	al_identity_transform(this);
}

al::Transform::Transform(const ALLEGRO_TRANSFORM* src)
{
	al_copy_transform(this, src);
}
		
void al::Transform::use() const
{
	al_use_transform(this);
}
void al::Transform::useProjection() const
{
	al_use_projection_transform(this);
}

al::Transform al::Transform::Eye()
{
	return Transform();
}
al::Transform al::Transform::Identity()
{
	return Transform();
}

al::Transform al::Transform::Build(float theta, al::Vec2<float> scale, al::Vec2<float> translation)
{
	Transform ret {Transform::NoInit()};
	al_build_transform(&ret, translation.x, translation.y, scale.x, scale.y, theta);
	return ret;
}
al::Transform al::Transform::Camera(al::Vec3<float> pos, al::Vec3<float> look, al::Vec3<float> up)
{
	Transform ret {Transform::NoInit()};
	al_build_camera_transform(
		&ret, 
		pos.x, pos.y, pos.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z
	);
	return ret;
}
al::Transform al::Transform::Orthographic(al::Vec2<float> leftTop, al::Vec2<float> rightBottom, float near, float far)
{
	Transform ret;
	al_orthographic_transform(
		&ret,
		leftTop.x, leftTop.y, near,
		rightBottom.x, rightBottom.y, far
	);
	return ret;
}
al::Transform al::Transform::Perspective(al::Vec2<float> leftTop, al::Vec2<float> rightBottom, float near, float far)
{
	Transform ret;
	al_perspective_transform(
		&ret,
		leftTop.x, leftTop.y, near,
		rightBottom.x, rightBottom.y, far
	);
	return ret;
}


al::Transform al::Transform::PerspectiveFOV(float aspect, float vFOV, float near, float far)
{
	if(vFOV <= 0.0f || vFOV >= 180.0f) {
		throw std::invalid_argument(fmt::format("Invalid vertical FOV value: {:.2f} degrees", vFOV));
	}
	float h = near * std::tan(vFOV*0.5 * DEG2RAD);
	float w = h*aspect;
	return Perspective(
		{-w,-h},
		{w,h},
		near,
		far
	);
}

al::Transform al::Transform::PerspectiveHFOV(float aspect, float hFOV, float near, float far)
{
	if(hFOV <= 0.0f || hFOV >= 180.0f) {
		throw std::invalid_argument(fmt::format("Invalid horizontal FOV value: {:.2f} degrees", hFOV));
	}
	
	float vFOV = 2.0f * std::atan(std::tan(vFOV*0.5 * DEG2RAD) / aspect) * RAD2DEG;
	return PerspectiveFOV(aspect, vFOV, near, far);
}

al::Transform& al::Transform::invert()
{
	al_invert_transform(this);
	return *this;
}
al::Transform& al::Transform::transpose()
{
	al_transpose_transform(this);
	return *this;
}

al::Transform& al::Transform::translate(al::Vec2<float> v)
{
	al_translate_transform(this, v.x, v.y);
	return *this;
}
al::Transform& al::Transform::scale(al::Vec2<float> scale)
{
	al_scale_transform(this, scale.x, scale.y);
	return *this;
}
al::Transform& al::Transform::rotate(float theta)
{
	al_rotate_transform(this, theta);
	return *this;
}

al::Transform& al::Transform::translate(al::Vec3<float> v)
{
	al_translate_transform_3d(this, v.x, v.y, v.z);
	return *this;
}
al::Transform& al::Transform::scale(al::Vec3<float> scale)
{
	al_scale_transform_3d(this, scale.x, scale.y, scale.z);
	return *this;
}
al::Transform& al::Transform::rotate(al::Vec3<float> center, float theta)
{
	al_rotate_transform_3d(this, center.x, center.y, center.z, theta);
	return *this;
}

al::Transform& al::Transform::horizontalShear(float theta)
{
	al_horizontal_shear_transform(this, theta);
	return *this;
}
al::Transform& al::Transform::verticalShear(float theta)
{
	al_vertical_shear_transform(this, theta);
	return *this;
}

al::Coord2<float> al::Transform::transform(al::Coord2<float> v) const
{
	al::Coord2<float> ret(v);
	al_transform_coordinates(this, &ret.x, &ret.y);
	return ret;
}
al::Coord3<float> al::Transform::transform(al::Coord3<float> v) const
{
	al::Coord3<float> ret(v);
	al_transform_coordinates_3d(this, &ret.x, &ret.y, &ret.z);
	return ret;
}
al::Coord4<float> al::Transform::transform(al::Coord4<float> v) const
{
	al::Coord4<float> ret(v);
	al_transform_coordinates_4d(this, &ret.x, &ret.y, &ret.z, &ret.w);
	return ret;
}

al::Coord3<float> al::Transform::transformProjective(al::Coord3<float> v) const
{
	al::Coord3<float> ret(v);
	al_transform_coordinates_3d_projective(this, &ret.x, &ret.y, &ret.z);
	return ret;
}


al::Transform& al::Transform::compose(const al::Transform& other)
{
	al_compose_transform(this, &other);
	return *this;
}


bool al::Transform::checkInverse(float tolerance) const
{
	return al_check_inverse(this, tolerance);
}

al::ScopedTransform::ScopedTransform(const al::Transform& t)
	: originalTransform(al_get_current_transform())
{
	al_use_transform(&t);
}

al::ScopedTransform::~ScopedTransform()
{
	al_use_transform(&originalTransform);
}