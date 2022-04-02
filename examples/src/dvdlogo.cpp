#include <axxegro/axxegro.hpp>

#include <algorithm>

/**
 * @file
 * 
 * A simple bouncing DVD logo example.
 */

int main()
{
	al::FullInit();
	al::Display disp(1024, 768, ALLEGRO_MIN_LINEAR|ALLEGRO_MIPMAP);

	al::Bitmap dvdLogo("data/dvdlogo.png");
	
	//position in px, speed in px/s
	al::Point pos(40,70), speed(256, 256);
	
	//size for both the bitmap and the collision box
	al::Vec2 logoSize {128, 128};

	/* create a basic event loop, which comes pre-configured
	 * with i/o event sources and a proper handler for the
	 * window close-event */
	al::EventLoop evLoop = al::EventLoop::Basic();
	
	/* create the loop body */
	evLoop.loopBody = [&](){
		al::CurrentDisplay().clearToColor(al::Color::RGB(150,180,240));

		//calculate current bounding box
		al::Rect logoRect = {pos, pos+logoSize};
		
		dvdLogo.drawScaled(dvdLogo.getRect(), logoRect);
		
		pos += speed * evLoop.getLastTickTime();

		//bouncing
		if(logoRect.b.y > al::CurrentDisplay().height() || logoRect.a.y < 0.0f) {
			pos.y = std::clamp(pos.y, 0.0f, al::CurrentDisplay().height()-logoSize.y);
			speed.y *= -1.0f;
		}
		if(logoRect.b.x > al::CurrentDisplay().width() || logoRect.a.x < 0.0f) {
			pos.x = std::clamp(pos.x, 0.0f, al::CurrentDisplay().width()-logoSize.x);
			speed.x *= -1.0f;
		}
		
		al::CurrentDisplay().flip();
	};

	//enable the frame limiter (240 fps)
	evLoop.enableClock(240);

	evLoop.run();
}