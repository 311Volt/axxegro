#ifndef INCLUDE_AXXEGRO_TRANSFORM
#define INCLUDE_AXXEGRO_TRANSFORM

#include <allegro5/allegro.h>

#include <axxegro/math/math.hpp>

namespace al {
	class Transform: public ALLEGRO_TRANSFORM {
	public:
		Transform();
		
		void use() const;
		void useProjection() const;

		static Transform GetCurrent();
		static Transform GetCurrentProjection();

		static Transform Eye();
		static Transform Identity();

		static Transform Build(float theta, const Vec2<float>& scale, const Vec2<float>& translation);
		static Transform Camera(const Vec3<float>& pos, const Vec3<float>& look, const Vec3<float>& up);
		static Transform Orthographic(const Vec2<float>& leftTop, const Vec2<float> rightBottom, float near, float far);
		static Transform Perspective(const Vec2<float>& leftTop, const Vec2<float> rightBottom, float near, float far);

		Transform& invert();
		Transform& transpose();

		Transform& translate(const Vec2<float>& v);
		Transform& scale(const Vec2<float>& scale);
		Transform& rotate(float theta);

		Transform& translate(const Vec3<float>& v);
		Transform& scale(const Vec3<float>& scale);
		Transform& rotate(const Vec3<float>& center, float theta);

		Transform& horizontalShear(float theta);
		Transform& verticalShear(float theta);
		
		Coord2<float> transform(const Coord2<float>& v) const;
		Coord3<float> transform(const Coord3<float>& v) const;
		Coord4<float> transform(const Coord4<float>& v) const;

		Coord3<float> transformProjective(const Coord3<float>& v) const;

		Coord2<float> operator()(const Coord2<float>& v) const 
			{return transform(v);}
		Coord3<float> operator()(const Coord3<float>& v) const
			{return transform(v);}
		Coord4<float> operator()(const Coord4<float>& v) const 
			{return transform(v);}
		
		Transform& compose(const Transform& other);

		Transform& operator*=(const Transform& other) {return compose(other);}
		Transform operator*(const Transform& rhs) {Transform n(*this); n*=rhs; return n;}

		bool checkInverse(float tolerance) const;

	#ifdef AXXEGRO_TRUSTED
		const ALLEGRO_TRANSFORM* alPtr(){return this;}
	#else
		ALLEGRO_TRANSFORM* alPtr(){return this;}
	#endif

	private:
		class NoInit{};
		Transform(NoInit) {}
		Transform(const ALLEGRO_TRANSFORM* src);
		friend class ScopedTransform;
	};

	class ScopedTransform {
		Transform originalTransform;
	public:
		ScopedTransform(ALLEGRO_TRANSFORM* t);
		ScopedTransform(const Transform& t);
		~ScopedTransform();
	};
}

#endif /* INCLUDE_AXXEGRO_TRANSFORM */
