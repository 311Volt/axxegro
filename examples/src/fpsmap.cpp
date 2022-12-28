#include <axxegro/axxegro.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <cmath>

using namespace al::ColorLiterals;

float Interpolate(float a, float b, float weight) 
{
	weight = std::clamp(weight, 0.0f, 1.0f);
    return (b - a) * (3.0 - weight * 2.0) * weight * weight + a;
}

al::Vec2f RandomGradient(al::Vec2<int> seed)
{
	uint32_t a = seed.x * 89733 + seed.y * 2879327 + 0xB16B00B5;
	a = a<<13 | a>>19;
	a *= 0xD398A93F;
	return al::Vec2f(std::cos(a), std::sin(a));
}

float Perlin(al::Vec2f pos)
{
	al::Vec2f floorPos(std::floor(pos.x), std::floor(pos.y));
	auto interp = pos - floorPos;

	al::Vec2f ipos[4] = {
		floorPos + al::Vec2f{0,0},
		floorPos + al::Vec2f{1,0},
		floorPos + al::Vec2f{0,1},
		floorPos + al::Vec2f{1,1}
	};

	float n[4];
	for(int i=0; i<4; i++) {
		n[i] = RandomGradient(ipos[i]).dot(pos-ipos[i]);
	}

	float ix0 = Interpolate(n[0], n[1], interp.x);
	float ix1 = Interpolate(n[2], n[3], interp.x);
	

	return std::clamp(Interpolate(ix0, ix1, interp.y), -1.0f, 1.0f) * 0.5 + 0.5;
}

float PerlinFractal(al::Vec2f pos, int octaves)
{
	octaves = std::max(1, octaves);
	float sum = 0;
	for(int i=0; i<octaves; i++) {
		int mul = 1 << i;
		sum += Perlin(pos * mul) / mul;
	}
	return sum / 2;
}

al::Bitmap PerlinNoise(int width, int height)
{
	al::Bitmap result(width, height);
	al::BitmapLockedRegion region(result, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READWRITE);

	for(int y=0; y<height; y++) {
		uint32_t* row = region.rowData<uint32_t>(y);
		for(int x=0; x<width; x++) {
			row[x] = al::Gray(PerlinFractal(al::Vec2f(x, y) * 0.01, 6)).rgba_u32();
		}
	}

	return result;
}

struct Mesh 
{
	std::vector<al::Vertex> vertices;
	std::vector<int> indices;
};


Mesh CreateTerrain(int sx, int sy, float height)
{
	Mesh result;
	for(int y=0; y<sy; y++) {
		for(int x=0; x<sx; x++) {
			al::Vec2f pos(x, y);
			float vh = height*PerlinFractal(pos*0.01, 8);
			al::Vertex vtx({pos.x, pos.y, vh}, pos, al::Gray(vh/height));
			result.vertices.push_back(vtx);
		}
	}

	for(int y=0; y<sy-1; y++) {
		for(int x=0; x<sx-1; x++) {
			int base = y*sx + x;
			int indices[6] = {base + 0, base + 1, base + sx, base + 1, base + sx + 1, base + sx};
			for(auto i: indices) {
				result.indices.push_back(i);
			}
		}
	}


	return result;
}

struct Camera {
	al::Vec3f pos;
	al::Vec2f rot;
	constexpr static al::Vec3f Up {0,0,1};

	al::Vec3f forward() {
		return al::Vec3f(
			std::cos(-rot.y) * std::cos(rot.x),
			std::sin(-rot.y) * std::cos(rot.x),
			std::sin(rot.x)
		).normalized();
	}

	void rotate(al::Vec2f delta) {
		rot += delta;
		rot.x = std::clamp(rot.x, -1.5706f, 1.5706f);
	}

	al::Vec3f right() {
		return forward().cross(Up).normalized();
	}

	al::Transform transform() {
		return al::Transform::Camera(pos, pos+forward(), Up);
	}
};


int main()
{
	al_set_config_value(al_get_system_config(), "trace", "level", "debug");
	std::set_terminate(al::Terminate);
	al::FullInit();

	al::Display display(1024, 768, 0, {}, {
		{ALLEGRO_DEPTH_SIZE, 32}, 
		{ALLEGRO_FLOAT_DEPTH, 1},
		{ALLEGRO_OPENGL_MAJOR_VERSION, 2}
	});

	Mesh terrain = CreateTerrain(200, 200, 64);
	fmt::print("{}_v {}_i\n", terrain.vertices.size(), terrain.indices.size());

	Camera camera;
	camera.pos = {-30, -30, 0};

	al::Transform proj = al::Transform::PerspectiveFOV(78, 0.01, 10000);

	al::EventLoop loop = al::EventLoop::Basic();


	loop.enableEscToQuit();
	loop.loopBody = [&](){

		al::Vec2f scrCenter = display.size() / 2.0;
		camera.rotate(al::Vec2f(al::GetMousePos() - scrCenter).transposed() * 0.002f);
		al::SetMousePos(scrCenter);

		float movDelta = loop.getLastTickTime() * 70.0f;
		auto keyb = al::GetKeyboardState();
		if(al::IsKeyDown(keyb, ALLEGRO_KEY_W)) {
			camera.pos += camera.forward() * movDelta;
		} 
		if(al::IsKeyDown(keyb, ALLEGRO_KEY_S)) {
			camera.pos -= camera.forward() * movDelta;
		} 
		if(al::IsKeyDown(keyb, ALLEGRO_KEY_A)) {
			camera.pos -= camera.right() * movDelta;
		} 
		if(al::IsKeyDown(keyb, ALLEGRO_KEY_D)) {
			camera.pos += camera.right() * movDelta;
		}

		al::TargetBitmap.clearToColor(al::Blue);
		al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
		al_clear_depth_buffer(1.0);
		camera.transform().use();
		proj.useProjection();

		al::DrawIndexedPrim(terrain.vertices, terrain.indices);

		al::CurrentDisplay.flip();
	};
	loop.enableFramerateLimit(60.0);
	loop.run();
}
