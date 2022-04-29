#ifndef INCLUDE_AXXEGRO_PRIM_HLDR
#define INCLUDE_AXXEGRO_PRIM_HLDR

#include <axxegro/math/math.hpp>
#include <axxegro/Color.hpp>

#include <vector>
#include <array>

/**
 * @file
 * high level drawing routines
 */

namespace al {
	constexpr al::Color PRIM_DEFAULT_COLOR = al::RGB(255,255,255);
	constexpr float PRIM_DEFAULT_THICKNESS = 1.0f;

	void DrawLine(
		const Coord<>& a, 
		const Coord<>& b, 
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawTriangle(
		const Coord<>& a, 
		const Coord<>& b, 
		const Coord<>& c, 
		const Color& color = PRIM_DEFAULT_COLOR, 
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledTriangle(
		const Coord<>& a, 
		const Coord<>& b, 
		const Coord<>& c, 
		const Color& color = PRIM_DEFAULT_COLOR
	);

	void DrawRectangle(
		const Rect<>& r,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledRectangle(
		const Rect<>& rect,
		const Color& color = PRIM_DEFAULT_COLOR
	);

	void DrawRoundRect(
		const Rect<>& rect,
		const Vec2<>& radius = {0, 0},
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledRoundRect(
		const Rect<>& rect,
		const Vec2<>& radius = {0, 0},
		const Color& color = PRIM_DEFAULT_COLOR
	);

	void DrawPieslice(
		const Coord<>& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledPieslice(
		const Coord<>& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR
	);

	void DrawEllipse(
		const Coord<>& center,
		const Vec2<>& radius,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledEllipse(
		const Coord<>& center,
		const Vec2<>& radius,
		const Color& color = PRIM_DEFAULT_COLOR
	);

	void DrawCircle(
		const Coord<>& center,
		float radius,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawFilledCircle(
		const Coord<>& center,
		float radius,
		const Color& color = PRIM_DEFAULT_COLOR
	);
	
	void DrawArc(
		const Coord<>& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);

	void DrawEllipticalArc(
		const Coord<>& center,
		const Vec2<>& radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);
	
	void DrawSpline(
		const std::array<Coord<>, 4>& points,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	);
	
	std::vector<Coord<>> CalculateArc(
		const Coord<>& center,
		const Coord<>& radius,
		float startTheta,
		float deltaTheta,
		float thickness,
		unsigned numPoints
	);

	//TODO ribbons
}
#endif /* INCLUDE_AXXEGRO_PRIM_HLDR */
