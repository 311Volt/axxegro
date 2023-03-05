#ifndef INCLUDE_AXXEGRO_TRANSFORM
#define INCLUDE_AXXEGRO_TRANSFORM

#include <allegro5/allegro.h>

#include <axxegro/math/math.hpp>
#include "Exception.hpp"


namespace al {
	constexpr double RAD2DEG = 180.0 / ALLEGRO_PI;
	constexpr double DEG2RAD = 1.0 / RAD2DEG;
	/**
	 * @brief Wraps around ALLEGRO_TRANSFORM.
	 * A pointer to al::Transform is also a valid pointer
	 * to ALLEGRO_TRANSFORM and the types seamlessly convert
	 * between each other.
	 */
	class Transform: public ALLEGRO_TRANSFORM {
	public:
		/// @brief Constructs an identity transform.
		Transform() : ALLEGRO_TRANSFORM() {
			al_identity_transform(this);
		}

		/// @brief Constructs a transform from a pointer.
		Transform(const ALLEGRO_TRANSFORM* src) : ALLEGRO_TRANSFORM() {
			al_copy_transform(this, src);
		}
		
		/// @brief Shorthand for al::TargetBitmap::useTransform(*this).
		void use() const {
			al_use_transform(this);
		}

		/// @brief Shorthand for al::TargetBitmap::useProjectionTransform(*this).
		void useProjection() const {
			al_use_projection_transform(this);
		}
		
		/**
		 * @brief Identity transform. 
		 * 
		 * @return A default-initialized (identity) transform.
		 */
		static Transform Eye() {
			return {};
		}

		/// @brief Same as Eye().
		static Transform Identity() {
			return Eye();
		}

		/// @brief Shorthand for Eye().rotate(theta).scale(scale).translate(translation).
		static Transform Build(float theta, Vec2<float> scale, Vec2<float> translation) {
			Transform ret {Transform::NoInit()};
			al_build_transform(&ret, translation.x, translation.y, scale.x, scale.y, theta);
			return ret;
		}

		/// @brief See al_build_camera_transform.
		static Transform Camera(Vec3<float> pos, Vec3<float> look, Vec3<float> up) {
			Transform ret {Transform::NoInit()};
			al_build_camera_transform(
					&ret,
					pos.x, pos.y, pos.z,
					look.x, look.y, look.z,
					up.x, up.y, up.z
			);
			return ret;
		}

		/// @brief See al_orthographic_transform.
		static Transform Orthographic(Vec2<float> leftTop, Vec2<float> rightBottom, float near, float far) {
			Transform ret;
			al_orthographic_transform(
					&ret,
					leftTop.x, leftTop.y, near,
					rightBottom.x, rightBottom.y, far
			);
			return ret;
		}

		/// @brief See al_perspective_transform.
		static Transform Perspective(Vec2<float> leftTop, Vec2<float> rightBottom, float near, float far) {
			Transform ret;
			al_perspective_transform(
					&ret,
					leftTop.x, leftTop.y, near,
					rightBottom.x, rightBottom.y, far
			);
			return ret;
		}

		static Transform PerspectiveHFOV(float hFOV, float near, float far, float aspect) {
			if(hFOV <= 0.0f || hFOV >= 180.0f) {
				throw InvalidTransformError("Invalid horizontal FOV value: {:.2f} degrees", hFOV);
			}

			auto vFOV = float(2.0 * std::atan(std::tan(hFOV*0.5 * DEG2RAD) / aspect) * RAD2DEG);
			return PerspectiveFOV(vFOV, near, far, aspect);
		}

		static Transform PerspectiveFOV(float vFOV, float near, float far, float aspect) {
			if(vFOV <= 0.0f || vFOV >= 180.0f) {
				throw InvalidTransformError("Invalid vertical FOV value: {:.2f} degrees", vFOV);
			}
			auto h = float(near * std::tan(vFOV*0.5 * DEG2RAD));
			auto w = float(h*aspect);
			return Perspective(
					{-w,-h},
					{w,h},
					near,
					far
			);
		}
		
		/// @brief Invert the 2D transformation. checkInverse() can be used to check for singularity.
		Transform& invert() {
			al_invert_transform(this);
			return *this;
		}

		/// @brief Transposes the transformation matrix.
		Transform& transpose() {
			al_transpose_transform(this);
			return *this;
		}

		/// @brief Translates the transformation by a given 2D vector.
		Transform& translate(Vec2<float> v) {
			al_translate_transform(this, v.x, v.y);
			return *this;
		}

		/// @brief Scales the transformation by a given 2D vector.
		Transform& scale(Vec2<float> scale) {
			al_scale_transform(this, scale.x, scale.y);
			return *this;
		}

		/// @brief Rotates the transformation by a given 2D vector.
		Transform& rotate(float theta) {
			al_rotate_transform(this, theta);
			return *this;
		}

		/// @brief Translates the transformation by a given 3D vector.
		Transform& translate(Vec3<float> v) {
			al_translate_transform_3d(this, v.x, v.y, v.z);
			return *this;
		}
		
		/// @brief Scales the transformation by a given 3D vector.
		Transform& scale(Vec3<float> scale) {
			al_scale_transform_3d(this, scale.x, scale.y, scale.z);
			return *this;
		}

		/// @brief Rotates the transformation by a given 3D vector.
		Transform& rotate(Vec3<float> center, float theta) {
			al_rotate_transform_3d(this, center.x, center.y, center.z, theta);
			return *this;
		}
		
		/// @brief Apply a horizontal shear for a 2D transform.
		Transform& horizontalShear(float theta) {
			al_horizontal_shear_transform(this, theta);
			return *this;
		}

		/// @brief Apply a vertical shear for a 2D transform.
		Transform& verticalShear(float theta) {
			al_vertical_shear_transform(this, theta);
			return *this;
		}
		
		/// @brief Transforms 2D coordinates.
		[[nodiscard]] Coord2<float> transform(Coord2<float> v) const {
			al::Coord2<float> ret(v);
			al_transform_coordinates(this, &ret.x, &ret.y);
			return ret;
		}

		/// @brief Transforms 3D coordinates. Use transformProjective() for projection transforms.
		[[nodiscard]] Coord3<float> transform(Coord3<float> v) const {
			al::Coord3<float> ret(v);
			al_transform_coordinates_3d(this, &ret.x, &ret.y, &ret.z);
			return ret;
		}

		/// @brief Transforms 4D coordinates.
		[[nodiscard]] Coord4<float> transform(Coord4<float> v) const {
			al::Coord4<float> ret(v);
			al_transform_coordinates_4d(this, &ret.x, &ret.y, &ret.z, &ret.w);
			return ret;
		}

		/// @brief https://liballeg.org/a5docs/trunk/transformations.html#al_transform_coordinates_3d_projective
		[[nodiscard]] Coord3<float> transformProjective(const Coord3<float> v) const {
			al::Coord3<float> ret(v);
			al_transform_coordinates_3d_projective(this, &ret.x, &ret.y, &ret.z);
			return ret;
		}

		/// @brief Same as transform(Coord2<>).
		[[nodiscard]] Coord2<float> operator()(Coord2<float> v) const {
			return transform(v);
		}

		/// @brief Same as transform(Coord3<>). Use transformProjective() for projection transforms.
		[[nodiscard]] Coord3<float> operator()(Coord3<float> v) const {
			return transform(v);
		}

		/// @brief Same as transform(Coord4<>).
		[[nodiscard]] Coord4<float> operator()(Coord4<float> v) const {
			return transform(v);
		}
		
		/// @brief Multiplies the transformation matrix by `other`.
		Transform& compose(const Transform& other) {
			al_compose_transform(this, &other);
			return *this;
		}

		/// @brief Same as `compose()`.
		Transform& operator*=(const Transform& other) {
			return compose(other);
		}

		/// @returns The composition of two transformations.
		Transform operator*(const Transform& rhs) {
			Transform n(*this); n*=rhs; return n;
		}

		/**
		 * @brief Check if 2D transform is invertible.
		 * @return true if transform is invertible
		 * @return false if transform is not invertible
		 */
		[[nodiscard]] bool checkInverse(float tolerance = 1e-7) const {
			return al_check_inverse(this, tolerance);
		}

		[[nodiscard]] const ALLEGRO_TRANSFORM* alPtr() const {
			return this;
		}
		[[nodiscard]] ALLEGRO_TRANSFORM* alPtr() {
			return this;
		}

	private:
		class NoInit{};
		explicit Transform(NoInit) {}
		friend class ScopedTransform;
	};

	/**
	 * @brief RAII for setting transformations.
	 */
	class ScopedTransform {
		Transform originalTransform;
	public:
		/// @brief Set the transformation given in the parameter as current and save the old one.
		explicit ScopedTransform(const Transform& t)
				: originalTransform(al_get_current_transform())
		{
			al_use_transform(&t);
		}

		/// @brief Set the transformation given in the parameter as current and save the old one.
		explicit ScopedTransform(ALLEGRO_TRANSFORM* t)
				: originalTransform(al_get_current_transform())
		{
			al_use_transform(t);
		}

		/// @brief Restore the transformation that was saved on construction.
		~ScopedTransform() {
			al_use_transform(&originalTransform);
		}
	};
}

#endif /* INCLUDE_AXXEGRO_TRANSFORM */
