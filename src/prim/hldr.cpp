#include <axxegro/prim/hldr.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


void al::DrawLine(
	const al::Coord<>& a, 
	const al::Coord<>& b, 
	const Color& color,
	float thickness
)
{
	al_draw_line(a.x, a.y, b.x, b.y, color, thickness);
}

void al::DrawTriangle(
	const al::Coord<>& a, 
	const al::Coord<>& b, 
	const al::Coord<>& c, 
	const Color& color, 
	float thickness
)
{
	al_draw_triangle(a.x, a.y, b.x, b.y, c.x, c.y, color, thickness);
}

void al::DrawFilledTriangle(
	const al::Coord<>& a, 
	const al::Coord<>& b, 
	const al::Coord<>& c, 
	const Color& color
)
{
	al_draw_filled_triangle(a.x, a.y, b.x, b.y, c.x, c.y, color);
}

void al::DrawRectangle(
	const Rect<>& r,
	const Color& color,
	float thickness
)
{
	al_draw_rectangle(r.a.x, r.a.y, r.b.x, r.b.y, color, thickness);
}

void al::DrawFilledRectangle(
	const Rect<>& rect,
	const Color& color
)
{
	al_draw_filled_rectangle(rect.a.x, rect.a.y, rect.b.x, rect.b.y, color);
}

void al::DrawRoundRect(
	const Rect<>& rect,
	const Vec2<>& radius,
	const Color& color,
	float thickness
)
{
	al_draw_rounded_rectangle(
		rect.a.x, rect.a.y, rect.b.x, rect.b.y,
		radius.x, radius.y,
		color, thickness
	);
}

void al::DrawFilledRoundRect(
	const Rect<>& rect,
	const Vec2<>& radius,
	const Color& color
)
{
	al_draw_filled_rounded_rectangle(
		rect.a.x, rect.a.y, rect.b.x, rect.b.y,
		radius.x, radius.y,
		color
	);
}

void al::DrawPieslice(
	const al::Coord<>& center,
	float radius,
	float startTheta,
	float deltaTheta,
	const Color& color,
	float thickness
)
{
	al_draw_pieslice(
		center.x, center.y,
		radius,
		startTheta, deltaTheta,
		color, thickness
	);
}

void al::DrawFilledPieslice(
	const al::Coord<>& center,
	float radius,
	float startTheta,
	float deltaTheta,
	const Color& color
)
{
	al_draw_filled_pieslice(
		center.x, center.y,
		radius,
		startTheta, deltaTheta,
		color
	);
}

void al::DrawEllipse(
	const al::Coord<>& center,
	const Vec2<>& radius,
	const Color& color,
	float thickness
)
{
	al_draw_ellipse(
		center.x, center.y,
		radius.x, radius.y,
		color, thickness
	);
}

void al::DrawFilledEllipse(
	const al::Coord<>& center,
	const Vec2<>& radius,
	const Color& color
)
{
	al_draw_filled_ellipse(
		center.x, center.y,
		radius.x, radius.y,
		color
	);
}

void al::DrawCircle(
	const al::Coord<>& center,
	float radius,
	const Color& color,
	float thickness
)
{
	al_draw_circle(
		center.x, center.y,
		radius,
		color, thickness
	);
}

void al::DrawFilledCircle(
	const al::Coord<>& center,
	float radius,
	const Color& color
)
{
	al_draw_filled_circle(
		center.x, center.y,
		radius,
		color
	);
}

void al::DrawArc(
	const al::Coord<>& center,
	float radius,
	float startTheta,
	float deltaTheta,
	const Color& color,
	float thickness
)
{
	al_draw_arc(
		center.x, center.y,
		radius,
		startTheta, deltaTheta,
		color, thickness
	);
}

void al::DrawEllipticalArc(
	const al::Coord<>& center,
	const Vec2<>& radius,
	float startTheta,
	float deltaTheta,
	const Color& color,
	float thickness
)
{
	al_draw_elliptical_arc(
		center.x, center.y,
		radius.x, radius.y,
		startTheta, deltaTheta,
		color, thickness
	);
}

void al::DrawSpline(
	const std::array<al::Coord<>, 4>& points,
	const Color& color,
	float thickness
)
{
	std::vector<float> pts(8);
	for(unsigned i=0; i<points.size(); i++) {
		pts[i*2 + 0] = points[i].x;
		pts[i*2 + 1] = points[i].y;
	}
	al_draw_spline(pts.data(), color, thickness);
}

std::vector<al::Coord<>> al::CalculateArc(
	const al::Coord<>& center,
	const al::Coord<>& radius,
	float startTheta,
	float deltaTheta,
	float thickness,
	unsigned numPoints
)
{
	std::vector<float> outData(2*numPoints*(1 + !!(thickness>0)));
	al_calculate_arc(
		outData.data(), 2*sizeof(outData[0]), 
		center.x, center.y,
		radius.x, radius.y,
		startTheta, deltaTheta,
		thickness,
		numPoints
	);
	std::vector<Coord<>> ret(outData.size() / 2);
	for(unsigned i=0; i<ret.size(); i++) {
		ret[i] = {outData[i*2 + 0], outData[i*2 + 1]};
	}
	return ret;
}