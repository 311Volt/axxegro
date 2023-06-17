
#include <axxegro/axxegro.hpp>

/**
 * @file
 * 
 * a simple 3d camera example
 */


std::array<al::Vertex, 3> DemoTriangle = {
	al::Vertex({0, 0, 0}, {0, 0}),
	al::Vertex({0, 1.5, 0}, {0, 1000}),
	al::Vertex({1.5, 0, 1.5}, {1000, 0})
};

int main()
{
	al::Display disp(800, 600);

	auto skyColor = al::RGB(100, 190, 240);

	// 78 FOV, view range 0.01 - 100
	auto proj = al::Transform::PerspectiveFOV(78, 0.01, 100.0, al::CurrentDisplay.aspectRatio());
	proj.useProjection();

	const auto& font = al::Font::BuiltinFont();

	//player orientation (pitch) in degrees
	float rx = 0.0;

	al::Bitmap tex("data/bg.jpg");

	//position and forward-facing vector
	al::Vec3f pos{0,0,-5}, fwd{0,0,1};

	auto evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();

	evLoop.loopBody = [&](){
		al::TargetBitmap.clearToColor(skyColor);
		double dt = evLoop.getLastTickTime();

		if(al::IsKeyDown(ALLEGRO_KEY_LEFT))
			rx += dt * 100.0f;
		if(al::IsKeyDown(ALLEGRO_KEY_RIGHT))
			rx -= dt * 100.0f;
		if(al::IsKeyDown(ALLEGRO_KEY_UP))
			pos += fwd * dt * 5.0f;
		if(al::IsKeyDown(ALLEGRO_KEY_DOWN))
			pos -= fwd * dt * 5.0f;
		
		//calculate forward direction based on orientation
		fwd = {std::sin(rx * float(al::DEG2RAD)), 0.0f, std::cos(rx * float(al::DEG2RAD))};

		//render HUD
		al::TargetBitmap.resetTransform();
		al::TargetBitmap.resetProjection();
		font.draw(
			al::Format(
				"player pos = (%.2f, %.2f, %.2f), facing %.2f deg", 
				pos.x, pos.y, pos.z, rx
			), 
			al::White, {10, 10}
		);
		
		//render 3D scene (in this case, one triangle)
		al::Transform::Camera(pos, pos+fwd, {0,1,0}).use();
		proj.useProjection();
		al::DrawPrim(DemoTriangle, tex);
		al::CurrentDisplay.flip();
	};
	
	evLoop.enableFramerateLimit();
	evLoop.run();
}
