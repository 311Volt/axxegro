#include <axxegro/axxegro.hpp>

#include <cmath>
#include <iostream>


/**
 * @file
 * The file where I test random things I implement. I guess it can showcase some
 * axxegro features, but for clearer and better explained examples, see other
 * files from this folder.
 *
 * Eventually, this file will be removed in favor of focused examples.
 */


int main()
{
	std::set_terminate(al::Terminate);
	al::Display disp(1024, 768, ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	al::Bitmap::SetNewBitmapFlags(ALLEGRO_MIPMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al::Bitmap bg("data/bg.jpg");
	al::Font font("data/roboto.ttf", 24);

	al::EventLoop loop(al::DemoEventLoopConfig);

	al::Vec2f txtPos {320, 240};
	std::string txtTest = al::Format("kb %dB, m %dB", (int)sizeof(ALLEGRO_KEYBOARD_STATE), (int)sizeof(ALLEGRO_MOUSE_STATE));

	loop.eventDispatcher.onMouseDown(al::LMB, [&](const al::MouseEvent& ev) {
		txtPos = {ev.x, ev.y};
	});


	loop.eventDispatcher.onKeyDown(ALLEGRO_KEY_UP, [&](){
		txtTest = "UP was pressed";
	});
	loop.eventDispatcher.onKeyDown(ALLEGRO_KEY_DOWN, [&](){
		txtTest = "DOWN was pressed";
	});
	loop.eventDispatcher.onKeyDown(ALLEGRO_KEY_LEFT, [&](){
		txtTest = "LEFT was pressed";
	});
	loop.eventDispatcher.onKeyDown(ALLEGRO_KEY_RIGHT, [&](){
		txtTest = "RIGHT was pressed";
	});

	loop.run([&](){
		al::TargetBitmap.clearToColor(al::RGB(0,0,0));

		float txtMaxWidth = 10.0 + (0.5+0.5*std::sin(al::GetTime())) * 300.0;
		std::string txtTest1 = al::Format("%s. tick=%d", txtTest.c_str(), (int)loop.getTick());
		std::string txtTestCut = txtTest1.substr(0, font.calcCutoffPoint(txtTest1, txtMaxWidth));

		{
			int y = loop.getTick() % (bg.height()-10);
			int x = loop.getTick() % (bg.width()-10);
			al::Vec2i p{x, y}, bb{2, 2};

			auto region = bg.lock<al::PixelRGB888>(al::RectI::PosSize(p, bb));

			for(int i=0; i<region.height(); i++) {
				auto row = region.row(i);
				for(auto& pixel: row) {
					pixel.g = 255;
				}
			}

		}

		bg.drawScaled(bg.rect(), al::CurrentDisplay.rect());


		{
			al::Transform t;
			t.translate(txtPos);
			al::ScopedTransform st(t);
			al::DrawLine({}, {txtMaxWidth,0.0f}, al::Red, 4.0);
			font.drawText(
				txtTestCut,
				al::RGB(255, 255, 255),
				{0, 0}
			);
		}

		auto r2 = al::Rect<int>::XYWH(70, 30, 80, 80);
		auto r1 = al::Rect<int>::XYWH(95, 160, 70, 70);
		auto r3 = r1.makeUnion(r2);
		al::DrawRectangle(r1, al::Blue);
		al::DrawRectangle(r2, al::Blue);
		al::DrawRectangle(r3, al::Magenta);


		for(int i=0; i<16; i++) {
			al::DrawFilledRectangle(al::RectI::XYWH(16*i, 0, 16, 16), al::CGA(i));
		}

		al::CurrentDisplay.flip();
	});
}
