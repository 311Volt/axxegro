#ifndef INCLUDE_AXXEGRO_TRANSFORM
#define INCLUDE_AXXEGRO_TRANSFORM

#include <allegro5/allegro.h>

#include <axxegro/math/math.hpp>


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
		Transform();

		/// @brief Constructs a transform from a pointer.
		Transform(const ALLEGRO_TRANSFORM* src);
		
		/// @brief Shorthand for al::TargetBitmap::useTransform(*this).
		void use() const;

		/// @brief Shorthand for al::TargetBitmap::useProjectionTransform(*this).
		void useProjection() const;
		
		/**
		 * @brief Identity transform. 
		 * 
		 * @return A default-initialized (identity) transform.
		 */
		static Transform Eye();

		/// @brief Same as Eye().
		static Transform Identity();

		/// @brief Shorthand for Eye().rotate(theta).scale(scale).translate(translation).
		static Transform Build(float theta, Vec2<float> scale, Vec2<float> translation);

		/// @brief See al_build_camera_transform.
		static Transform Camera(Vec3<float> pos, Vec3<float> look, Vec3<float> up);

		/// @brief See al_orthographic_transform.
		static Transform Orthographic(Vec2<float> leftTop, Vec2<float> rightBottom, float near, float far);

		/// @brief See al_perspective_transform.
		static Transform Perspective(Vec2<float> leftTop, Vec2<float> rightBottom, float near, float far);

		static Transform PerspectiveHFOV(float aspect, float hFOV, float near, float far);

		static Transform PerspectiveFOV(float aspect, float vFOV, float near, float far);
		
		/// @brief Invert the 2D transformation. checkInverse() can be used to check for singularity.
		Transform& invert();

		/// @brief Transposes the transformation matrix.
		Transform& transpose();

		/// @brief Translates the transformation by a given 2D vector.
		Transform& translate(Vec2<float> v);

		/// @brief Scales the transformation by a given 2D vector.
		Transform& scale(Vec2<float> scale);

		/// @brief Rotates the transformation by a given 2D vector.
		Transform& rotate(float theta);

		/// @brief Translates the transformation by a given 3D vector.
		Transform& translate(Vec3<float> v);
		
		/// @brief Scales the transformation by a given 3D vector.
		Transform& scale(Vec3<float> scale);

		/// @brief Rotates the transformation by a given 3D vector.
		Transform& rotate(Vec3<float> center, float theta);
		
		/// @brief Apply a horizontal shear for a 2D transform.
		Transform& horizontalShear(float theta);

		/// @brief Apply a vertical shear for a 2D transform.
		Transform& verticalShear(float theta);
		
		/// @brief Transforms 2D coordinates.
		Coord2<float> transform(Coord2<float> v) const;

		/// @brief Transforms 3D coordinates. Use transformProjective() for projection transforms.
		Coord3<float> transform(Coord3<float> v) const;

		/// @brief Transforms 4D coordinates.
		Coord4<float> transform(Coord4<float> v) const;

		/// @brief https://liballeg.org/a5docs/trunk/transformations.html#al_transform_coordinates_3d_projective
		Coord3<float> transformProjective(const Coord3<float> v) const;

		/// @brief Same as transform(Coord2<>).
		Coord2<float> operator()(Coord2<float> v) const 
			{return transform(v);}

		/// @brief Same as transform(Coord3<>). Use transformProjective() for projection transforms.
		Coord3<float> operator()(Coord3<float> v) const
			{return transform(v);}

		/// @brief Same as transform(Coord4<>).
		Coord4<float> operator()(Coord4<float> v) const 
			{return transform(v);}
		
		/// @brief Multiplies the transformation matrix by `other`.
		Transform& compose(const Transform& other);

		/// @brief Same as `compose()`.
		Transform& operator*=(const Transform& other) {return compose(other);}

		/// @returns The composition of two transformations.
		Transform operator*(const Transform& rhs) {Transform n(*this); n*=rhs; return n;}

		/**
		 * @brief Check if 2D transform is invertible.
		 * @return true if transform is invertible
		 * @return false if transform is not invertible
		 */
		bool checkInverse(float tolerance = 1e-7) const;

	#ifdef AXXEGRO_TRUSTED
		const ALLEGRO_TRANSFORM* alPtr(){return this;}
	#else
		ALLEGRO_TRANSFORM* alPtr(){return this;}
	#endif

	private:
		class NoInit{};
		Transform(NoInit) {}
		friend class ScopedTransform;
	};

	/**
	 * @brief RAII for setting transformations.
	 */
	class ScopedTransform {
		Transform originalTransform;
	public:
		/// @brief Set the transformation given in the parameter as current and save the old one.
		ScopedTransform(const Transform& t);
		/// @brief Set the transformation given in the parameter as current and save the old one.
		ScopedTransform(ALLEGRO_TRANSFORM* t);

		/// @brief Restore the transformation that was saved on construction.
		~ScopedTransform();
	};
}

#endif /* INCLUDE_AXXEGRO_TRANSFORM */
