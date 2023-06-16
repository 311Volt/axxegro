#ifndef INCLUDE_AXXEGRO_PRIM_HLDR
#define INCLUDE_AXXEGRO_PRIM_HLDR

#include "common.hpp"
#include "PrimitivesAddon.hpp"

#include <vector>
#include <array>

/**
 * @file
 * high level drawing routines
 */


namespace al {
	constexpr al::Color PRIM_DEFAULT_COLOR = al::RGB(255,255,255);
	constexpr float PRIM_DEFAULT_THICKNESS = 1.0f;

	inline void DrawLine(
		const Vec2f& a,
		const Vec2f& b,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_line(a.x, a.y, b.x, b.y, color, thickness);
	}

	inline void DrawTriangle(
		const Vec2f& a,
		const Vec2f& b,
		const Vec2f& c,
		const Color& color = PRIM_DEFAULT_COLOR, 
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_triangle(a.x, a.y, b.x, b.y, c.x, c.y, color, thickness);
	}

	inline void DrawFilledTriangle(
		const Vec2f& a,
		const Vec2f& b,
		const Vec2f& c,
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_triangle(a.x, a.y, b.x, b.y, c.x, c.y, color);
	}

	inline void DrawRectangle(
		const RectF& r,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_rectangle(r.a.x, r.a.y, r.b.x, r.b.y, color, thickness);
	}

	inline void DrawFilledRectangle(
		const RectF& rect,
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_rectangle(rect.a.x, rect.a.y, rect.b.x, rect.b.y, color);
	}

	inline void DrawRoundRect(
		const RectF& rect,
		const Vec2f& radius = {0, 0},
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_rounded_rectangle(
				rect.a.x, rect.a.y, rect.b.x, rect.b.y,
				radius.x, radius.y,
				color, thickness
		);
	}

	inline void DrawFilledRoundRect(
		const RectF& rect,
		const Vec2f& radius = {0, 0},
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_rounded_rectangle(
				rect.a.x, rect.a.y, rect.b.x, rect.b.y,
				radius.x, radius.y,
				color
		);
	}

	inline void DrawPieslice(
		const Vec2f& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_pieslice(
				center.x, center.y,
				radius,
				startTheta, deltaTheta,
				color, thickness
		);
	}

	inline void DrawFilledPieslice(
		const Vec2f& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_pieslice(
				center.x, center.y,
				radius,
				startTheta, deltaTheta,
				color
		);
	}

	inline void DrawEllipse(
		const Vec2f& center,
		const Vec2f& radius,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_ellipse(
				center.x, center.y,
				radius.x, radius.y,
				color, thickness
		);
	}

	inline void DrawFilledEllipse(
		const Vec2f& center,
		const Vec2f& radius,
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_ellipse(
				center.x, center.y,
				radius.x, radius.y,
				color
		);
	}

	inline void DrawCircle(
		const Vec2f& center,
		float radius,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_circle(
				center.x, center.y,
				radius,
				color, thickness
		);
	}

	inline void DrawFilledCircle(
		const Vec2f& center,
		float radius,
		const Color& color = PRIM_DEFAULT_COLOR
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_filled_circle(
				center.x, center.y,
				radius,
				color
		);
	}

	inline void DrawArc(
		const Vec2f& center,
		float radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_arc(
				center.x, center.y,
				radius,
				startTheta, deltaTheta,
				color, thickness
		);
	}

	inline void DrawEllipticalArc(
		const Vec2f& center,
		const Vec2f& radius,
		float startTheta,
		float deltaTheta,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		al_draw_elliptical_arc(
				center.x, center.y,
				radius.x, radius.y,
				startTheta, deltaTheta,
				color, thickness
		);
	}

	inline void DrawSpline(
		const std::array<Vec2f, 4>& points,
		const Color& color = PRIM_DEFAULT_COLOR,
		float thickness = PRIM_DEFAULT_THICKNESS
	) {
		InternalRequire<PrimitivesAddon>();
		std::vector<float> pts(8);
		for(unsigned i=0; i<points.size(); i++) {
			pts[i*2 + 0] = points[i].x;
			pts[i*2 + 1] = points[i].y;
		}
		al_draw_spline(pts.data(), color, thickness);
	}

	inline std::vector<Vec2f> CalculateArc(
		const Vec2f& center,
		const Vec2f& radius,
		float startTheta,
		float deltaTheta,
		float thickness,
		int numPoints
	) {
		InternalRequire<PrimitivesAddon>();
		std::vector<float> outData(2*numPoints*(1 + (thickness > 0)));
		al_calculate_arc(
				outData.data(), 2*sizeof(outData[0]),
				center.x, center.y,
				radius.x, radius.y,
				startTheta, deltaTheta,
				thickness,
				numPoints
		);
		std::vector<Vec2f> ret(outData.size() / 2);
		for(unsigned i=0; i<ret.size(); i++) {
			ret[i] = {outData[i*2 + 0], outData[i*2 + 1]};
		}
		return ret;
	}

	//TODO ribbons
}
#endif /* INCLUDE_AXXEGRO_PRIM_HLDR */
