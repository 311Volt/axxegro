#include <axxegro/axxegro.hpp>

/** @file
 * This example demonstrates how to use hardware GPU buffers and how to
 * modify their data on the fly. The amount of data written to the GPU per
 * frame is varied and an FPS counter is included to demonstrate the
 * performance impact of modifying a small portion of the buffer.
 *
 * The colored pattern visible when running the program is a 64x64 grid
 * of triangles, with vertex colors updating every frame.
 */

struct Vertex2D {
	al::Vec2f pos;
	al::Color color;
};

template<typename T>
struct Arr2D {
	explicit Arr2D(al::Vec2i size) : mSize(size), mData(size.x * size.y) {}
	[[nodiscard]] int indexAt(int x, int y) const {return y * mSize.x + x;}
	T& operator()(int x, int y) {return mData[indexAt(x, y)];}
	const T& operator()(int x, int y) const {return mData[indexAt(x, y)];}

	al::Vec2i mSize;
	std::vector<T> mData;
};

Arr2D<Vertex2D> GenerateGrid(int width, int height);
std::vector<int> GenerateIndices(const Arr2D<Vertex2D>& vtxs);
void UpdateColors(Arr2D<Vertex2D>& vtxs, double time, double extent);

double sin2(double x){return 0.5 + 0.5*std::sin(2.0*x);}

int main()
{
	al::Display display(800, 600);
	std::set_terminate(al::Terminate);

	auto grid = GenerateGrid(64, 64);
	auto idxs = GenerateIndices(grid);

	al::VertexBuffer vertexBuffer(grid.mData);
	al::IndexBuffer indexBuffer(idxs);

	al::EventLoop loop(al::DemoEventLoopConfig);
	loop.framerateLimiter.setLimit(al::FPSLimit::None);
	loop.fpsCounter.setInterval(0.25);

	auto font = al::Font::CreateBuiltinFont();

	loop.run([&](){
		al::TargetBitmap.clear();

		{
			auto extent = sin2(al::GetTime()); // a value of 0.2 means we only update the first 20%
			UpdateColors(grid, al::GetTime(), extent);
			auto lockSize = std::max<int>(1, vertexBuffer.size() * extent);


			/* Locking the vertex buffer. The lock() method returns an object
			 * that owns the lock, provides access to the data and will unlock
			 * the buffer once it goes out of scope. */

			auto vBufData = vertexBuffer.lock(0, lockSize);
			std::copy(grid.mData.begin(), grid.mData.begin()+lockSize, vBufData.data().begin());
		}

		al::Transform{}.scale(al::CurrentDisplay.size().f32()).use();
		al::DrawIndexedBuffer(vertexBuffer, indexBuffer);

		al::Transform::Eye().use();
		font.drawText(al::Format("%d fps", (int)loop.getFPS()), al::Black, {16, 16});
		font.drawText(al::Format("%d fps", (int)loop.getFPS()), al::White, {15, 15});

		al::CurrentDisplay.flip();
	});
}





////////////////////////////////////////////////////////////////////////////



Arr2D<Vertex2D> GenerateGrid(int width, int height) {
	Arr2D<Vertex2D> result({width, height});
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			result(x, y) = {.pos = {x / double(width), y / double(height)}, .color = al::White};
		}
	}
	return result;
}

std::vector<int> GenerateIndices(const Arr2D<Vertex2D>& vtxs) {
	std::vector<int> result;
	result.reserve(vtxs.mSize.foldl(1, std::multiplies<int>{}) * 6);
	for(int x=0; x<vtxs.mSize.x-1; x++) {
		for(int y=0; y<vtxs.mSize.y-1; y++) {
			int w = vtxs.mSize.x;
			for(int offset: {0, 1, w, 1, w+1, w}) {
				result.push_back(vtxs.indexAt(x, y) + offset);
			}
		}
	}
	return result;
}

void UpdateColors(Arr2D<Vertex2D>& vtxs, double t, double extent) {
	for(int x=0; x<vtxs.mSize.x; x++) {
		for(int y=0; y<vtxs.mSize.y * extent; y++) {
			float phi[3] = {0, ALLEGRO_PI / 6.0, 2.0*ALLEGRO_PI / 6.0};
			float base = x/20.0f + y/20.0f + t;
			vtxs(x, y).color = al::RGB_f(
				sin2(base + phi[0]),
				sin2(base + phi[1]),
				sin2(base + phi[2])
			);
		}
	}
}