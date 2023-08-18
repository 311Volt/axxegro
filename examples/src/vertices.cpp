
#include <axxegro/axxegro.hpp>

/**
 * @file
 * 
 * a quick example on how to create custom vertex declarations
 * and use them to draw primitives
 */

/*
 * The simplest way to create a custom vertex type is to declare a struct like this.
 * If axxegro can recognize the member names and types, it will create an
 * ALLEGRO_VERTEX_ELEMENT array automatically at compile time based on the struct.
 *
 * axxegro looks for members named "pos", "uvPx", "uvNorm", "normal", "rgba" or "color",
 * in that order (it matters when accesing the user attrs "normal" and "rgba" in shaders).
 */
struct MyVertex {
	al::Vec2f pos;
	al::Vec2f uvPx;
	al::Color color = al::White;
};
/*
 * This vertex format can now be used like this:
 *     al::DrawPrim<MyVertex>(vtxs, bg);
 *
 * The struct is checked for correctness at compile time, as far as it can.
 * If you get compiler errors, you'll be interested in the ConstevalFail() line for information
 * on your error.
 */



/*
 * The above is not sufficient if you want custom names or additional data.
 * In this case, you can specialize al::VertexAttrArrGetter to provide your own ALLEGRO_VERTEX_ELEMENT array.
 */
struct MyAdvancedVertex {
	al::Vec3f pos = {};
	al::Vec4f customdata = {};
};
namespace al {
	template<>
	struct VertexAttrArrGetter<MyAdvancedVertex> {
		static consteval auto GetElements() {
			return al::CreateVertexAttrArr({
				{ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_3, offsetof(MyAdvancedVertex, pos)},
				{ALLEGRO_PRIM_USER_ATTR + 0, ALLEGRO_PRIM_FLOAT_4, offsetof(MyAdvancedVertex, customdata)}
			});
		}
	};
}

int main()
{
	al::Display disp(800, 600);
	al::TargetBitmap.clearToColor(al::RGB(100,100,100));
	al::Bitmap bg = al::LoadBitmap("data/bg.jpg");

	std::vector<MyVertex> vtxs{
		MyVertex {.pos = {1,1}, .uvPx = {0,0}},
		MyVertex {.pos = {2,2}, .uvPx = {0,1000}, .color = al::Blue},
		MyVertex {.pos = {3,1}, .uvPx = {1000,0}}
	};

	std::vector<MyAdvancedVertex> vtxs2 = {
		MyAdvancedVertex {.pos = {4,1,0}},
		MyAdvancedVertex {.pos = {5,2,0}},
		MyAdvancedVertex {.pos = {6,1,0}}
	};

	al::Transform().scale({100, 100}).use();

	al::EventLoop loop(al::DemoEventLoopConfig);
	loop.setFramerateLimit(al::Hz(30));
	loop.run([&](){
		al::TargetBitmap.clear();

		al::DrawPrim<MyVertex>(vtxs, bg);
		al::DrawPrim<MyAdvancedVertex>(vtxs2, bg);

		al::CurrentDisplay.flip();
	});
}
