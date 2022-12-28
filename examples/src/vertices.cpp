
#include <axxegro/axxegro.hpp>

/**
 * @file
 * 
 * a quick example on how to create custom vertex declarations
 * and use them to draw primitives
 */

struct MyVertex {
	float x,y;
	float u,v;
};

//struct MyVertexDecl: public al::BasicVertexDecl<MyVertex> 

AXXEGRO_VERTEX_DECL(MyVertex) {
	AXXEGRO_VERTEX_ATTR_BEGIN()
	AXXEGRO_VERTEX_ATTR(x, ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_2)
	AXXEGRO_VERTEX_ATTR(u, ALLEGRO_PRIM_TEX_COORD_PIXEL, ALLEGRO_PRIM_FLOAT_2)
};

int main()
{
	al::FullInit();
	al::Display disp(800, 600);
	al::TargetBitmap.clearToColor(al::RGB(100,100,100));
	al::Bitmap bg("data/bg.jpg");

	std::vector<MyVertex> vtxs{
		{1,1, 0,0},
		{2,2, 0,1000},
		{3,1, 1000,0}
	};
	
	al::Transform().scale({100, 100}).use();
	al::DrawPrim<MyVertex>(vtxs, bg);
	al::CurrentDisplay.flip();

	al::Sleep(2.0);
}
