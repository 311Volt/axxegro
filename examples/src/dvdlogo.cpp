#include <axxegro/axxegro.hpp>


/**
 * @file
 * 
 * A simple bouncing DVD logo example.
 */

int main()
{
	al::Display disp(1024, 768, ALLEGRO_MIN_LINEAR|ALLEGRO_MIPMAP);

	al::Bitmap dvdLogo = al::LoadBitmap("data/dvdlogo.png");
	
	al::Vec2<float> speed(256, 256); // in px/s
	al::Vec2<float> logoSize(128, 128); // in px

	//screen rectangle ((0,0), (w,h))
	al::Rect<float> scrRect = al::CurrentDisplay.rect();

	//rectangle for the dvd logo (position/size)
	al::Rect<float> logoRect = al::Rect<float>::PosSize({40, 70}, logoSize);
	

	/* create a basic event loop, which comes pre-configured
	 * with i/o event sources and a proper handler for the
	 * window close event */
	al::EventLoop evLoop(al::DemoEventLoopConfig);


	evLoop.run([&](){
		al::TargetBitmap.clearToColor(al::RGB(150,180,240));

		dvdLogo.drawScaled(dvdLogo.rect(), logoRect);

		logoRect += speed * evLoop.getLastTickTime();

		//bouncing
		uint8_t test = scrRect.test(logoRect);
		if(test & al::RectF::TEST_X_NOT_IN_RANGE) {
			speed.x *= -1.0f;
		}
		if(test & al::RectF::TEST_Y_NOT_IN_RANGE) {
			speed.y *= -1.0f;
		}
		logoRect = scrRect.clamp(logoRect);

		al::CurrentDisplay.flip();
	});
}