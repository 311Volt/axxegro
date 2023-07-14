#include <axxegro/axxegro.hpp>
#include <random>
#include <algorithm>

/*
 * A benchmark template that doubles as an example program
 */

class Avg {
	double state = 0.0;
	int num = 0;
public:
	void add(double x) {state += x; num++;}
	[[nodiscard]] double getAvg() const {return num ? state/(double)num : 0.0;}
};

int main()
{
	al::RequireAllAddons();
	std::set_terminate(al::Terminate);

	al::Display disp(800, 600);
	
	al::EventLoop loop(al::DemoEventLoopConfig);
	
	auto font = al::Font::CreateBuiltinFont();
	static constexpr int BenchmarkTicks = 700;
	
	Avg frametimes;

	loop.run([&](){

		static std::random_device rd;
		static std::mt19937_64 gen(rd());
		static std::uniform_int_distribution<int> distW(0, al::CurrentDisplay.width());
		static std::uniform_int_distribution<int> distH(0, al::CurrentDisplay.height());

		al::TargetBitmap.clear();

		for(int i=0; i<100000; i++) {
			int x1 = distW(gen);
			int y1 = distH(gen);
			int x2 = distW(gen);
			int y2 = distH(gen);

			al::DrawLine(al::Vec2f(x1, y1), al::Vec2f(x2, y2), al::Blue);
		}

		if(loop.getTick() > 1) {
			frametimes.add(loop.getLastTickTime());
		}
		if(loop.getTick() >= BenchmarkTicks) {
			loop.setExitFlag();
		}
		font.drawText(
			al::Format("tick=%d, avg frametime: %.09f ms", (int) loop.getTick(), 1000.0 * frametimes.getAvg()),
			al::PureYellow, {100, 100});

		al::CurrentDisplay.flip();


	});
	
	printf("average frametime over %d ticks: %.09f ms", BenchmarkTicks, 1000.0 * frametimes.getAvg());
}

/*
 * avg frametime with auto init: 64.430803900 ms
 * avg frametime without auto init: 64.060008601 ms
 */