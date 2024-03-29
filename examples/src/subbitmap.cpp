
#include <axxegro/axxegro.hpp>

int main()
{
    std::set_terminate(al::Terminate);

    al::Display disp(800, 600);
    al::EventLoop loop(al::DemoEventLoopConfig);
    al::Bitmap parent = al::LoadBitmap("data/terrain.png");

    loop.run([&](){
		al::TargetBitmap.clear();

		int idx = int(al::GetTime() * 5.0) % 256;
		int x = idx % 16;
		int y = idx / 16;

		auto rect = al::Rect<int>::XYWH(x*16, y*16, 16, 16);
		auto sub = parent.createSubBitmap(rect);

		sub.drawScaled(sub.rect(), al::RectF::XYWH(100, 100, 64, 64));

		al::CurrentDisplay.flip();
	});

    return 0;
}