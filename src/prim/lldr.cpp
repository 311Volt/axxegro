#include <axxegro/prim/lldr.hpp>

al::Vertex::Vertex(const Vec3<> pos, const Vec2<> uv, Color color)
{
	setPos(pos);
	setUV(uv);
	setColor(color);
}

void al::Vertex::setPos(const al::Vec3<> pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}
void al::Vertex::setUV(const al::Vec2<> uv)
{
	u = uv.x;
	v = uv.y;
}
void al::Vertex::setColor(const al::Color color)
{
	this->color = color;
}

al::Vec3<> al::Vertex::getPos() const
{
	return {x,y,z};
}
al::Vec2<> al::Vertex::getUV() const
{
	return {u,v};
}
al::Color al::Vertex::getColor() const
{
	return this->color;
}