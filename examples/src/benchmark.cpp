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

struct SmallEvent {
	static constexpr al::EventType EventTypeID = 9999;
	int a=0, b=0;
};

struct LargeEvent {
	static constexpr al::EventType EventTypeID = 9997;
	static inline uint64_t count = 0;
	std::array<int, 32> elements;
};


void EventBenchmark() {
	al::EventQueue queue;
	al::EventDispatcher dispatcher;
	al::UserEventSource evSrc;
	queue.registerSource(evSrc);

	auto dSmall = dispatcher.addDiscretizer([](const al::Event& event) -> int64_t {
		return al::GetUserEventData<SmallEvent>(event).a;
	});
	auto dLarge = dispatcher.addDiscretizer([](const al::Event& event) -> int64_t {
		return al::GetUserEventData<LargeEvent>(event).elements[0];
	});

	for(int i=0; i<100; i += 2) {
		dispatcher.setEventHandlerForValue<SmallEvent>(SmallEvent::EventTypeID, dSmall, i, [](){});
		dispatcher.setEventHandlerForValue<LargeEvent>(LargeEvent::EventTypeID, dLarge, i, [](){});
	}
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	static std::uniform_int_distribution<int> distV(0, 100);

	static constexpr int BatchSize = 10000;
	static constexpr int BenchmarkSize = 6400000;
	static constexpr int NumBatches = BenchmarkSize / BatchSize;
	static_assert(BenchmarkSize % BatchSize == 0);

	Avg avgBatchEmitSmall, avgBatchEmitLarge, avgBatchDispatchSmall, avgBatchDispatchLarge;

	for(int i=0; i<NumBatches; i++) {
		if(i % 10 == 0)
			printf("%d\t", i);
		double st1 = al::GetTime();
		for(int j=0; j<BatchSize; j++) {
			evSrc.emitEvent(SmallEvent{distV(gen), 0});
		}
		double st2 = al::GetTime();
		avgBatchEmitSmall.add(st2 - st1);

		double sdt1 = al::GetTime();
		while(not queue.empty()) {
			auto event = queue.pop();
			dispatcher.dispatch(event.get());
		}
		double sdt2 = al::GetTime();
		avgBatchDispatchSmall.add(sdt2 - sdt1);

		double lt1 = al::GetTime();
		for(int j=0; j<BatchSize; j++) {
			LargeEvent ev = {};
			ev.elements[0] = distV(gen);
			evSrc.emitEvent(ev);
		}
		double lt2 = al::GetTime();
		avgBatchEmitLarge.add(lt2 - lt1);

		double sdl1 = al::GetTime();
		while(not queue.empty()) {
			auto event = queue.pop();
			dispatcher.dispatch(event.get());
		}
		double sdl2 = al::GetTime();
		avgBatchDispatchLarge.add(sdl2 - sdl1);
	}
	std::cout << std::format(
			"\n\nAverages per {}: \n"
			"small emit: {:.3f} ms\n"
			"large emit: {:.3f} ms\n"
			"small dispatch: {:.3f} ms\n"
			"large dispatch: {:.3f} ms\n",
			BatchSize,
			avgBatchEmitSmall.getAvg() * 1000.0,
			avgBatchEmitLarge.getAvg() * 1000.0,
			avgBatchDispatchSmall.getAvg() * 1000.0,
			avgBatchDispatchLarge.getAvg() * 1000.0);


}

int main()
{
	al::RequireAllAddons();
	std::set_terminate(al::Terminate);

	EventBenchmark();

	al::Display disp(800, 600);
	
	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();
	
	auto font = al::Font::CreateBuiltinFont();
	static constexpr int BenchmarkTicks = 700;
	
	Avg frametimes;
	
	loop.loopBody = [&](){
		
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
		font.draw(al::Format("tick=%d, avg frametime: %.09f ms", (int)loop.getTick(), 1000.0 * frametimes.getAvg()), al::PureYellow, {100, 100});
		
		al::CurrentDisplay.flip();
		
		
	};
	loop.run();
	
	printf("average frametime over %d ticks: %.09f ms", BenchmarkTicks, 1000.0 * frametimes.getAvg());
}

/*
 * avg frametime with auto init: 64.430803900 ms
 * avg frametime without auto init: 64.060008601 ms
 */