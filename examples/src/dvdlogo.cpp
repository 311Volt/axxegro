#include <axxegro/axxegro.hpp>

#include <algorithm>

int main()
{
	al::FullInit();
	al::Display disp(1024, 768, ALLEGRO_MIN_LINEAR|ALLEGRO_MIPMAP);
	al::Bitmap dvdLogo("data/dvdlogo.png");
	al::Point pos(40,70), speed(256, 256);
	al::Vec2 logoSize {128, 128};

	al::EventLoop evLoop = al::EventLoop::Basic();
	
	int cooldown = 0;

	evLoop.loopBody = [&](){
		al::CurrentDisplay().clearToColor(al::Color::RGB(150,180,240));

		al::Rect logoRect = {pos, pos+logoSize};
		dvdLogo.drawScaled(dvdLogo.getRect(), logoRect);
		pos += speed * evLoop.getLastTickTime();

		if(!cooldown) {
			if(logoRect.b.y > al::CurrentDisplay().height() || logoRect.a.y < 0.0f) {
				cooldown = 10;
				speed.y *= -1.0f;
			}
			if(logoRect.b.x > al::CurrentDisplay().width() || logoRect.a.x < 0.0f) {
				cooldown = 10;
				speed.x *= -1.0f;
			}
		}
		cooldown = std::max(0, cooldown-1);
		al::CurrentDisplay().flip();
	};

	evLoop.enableClock(240);
	evLoop.run();
}