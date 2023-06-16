#include <axxegro/axxegro.hpp>

#include <cmath>
#include <iostream>


/**
 * @file
 * The file where I test random things I implement. I guess it can showcase some
 * axxegro features, but for clearer and better explained examples, see other
 * files from this folder.
 */


int main()
{
	al::Display disp(1024, 768, ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	al::Bitmap::SetNewBitmapFlags(ALLEGRO_MIPMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al::Bitmap bg("data/bg.jpg");
	al::Font font("data/roboto.ttf", 24);

	al::Config cfg("data/samplecfg.ini");

	for(auto& section: cfg.sections()) {
		std::cout << al::Format("%s: \n", section.c_str());
		for(auto& key: cfg.keys(section)) {
			al::Config::SectionSelector ss(cfg, section);
			std::cout << al::Format("\t%s -> %s\n", key.c_str(), cfg.getValue(key).c_str());
		}
		std::cout << "\n\n";
	}

	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();

	al::Vec2f txtPos {320, 240};
	std::string txtTest = al::Format("kb %dB, m %dB", sizeof(ALLEGRO_KEYBOARD_STATE), sizeof(ALLEGRO_MOUSE_STATE));
	loop.loopBody = [&](){
		al::TargetBitmap.clearToColor(al::RGB(0,0,0));

		float txtMaxWidth = 10.0 + (0.5+0.5*std::sin(al::GetTime())) * 300.0;
		std::string txtTest1 = al::Format("%s. tick=%d", txtTest.c_str(), (int)loop.getTick());
		std::string txtTestCut = txtTest1.substr(0, font.calcCutoffPoint(txtTest1, txtMaxWidth));

		{
			int y = loop.getTick() % (bg.height()-10);
			int x = loop.getTick() % (bg.width()-10);
			al::Vec2i p{x, y}, bb{2, 2};
			al::BitmapLockedRegion lr(bg, {p, p+bb}, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READWRITE);
			lr.rowData<uint32_t>(1)[1] = 0xFF00FF;
		}

		bg.drawScaled(bg.rect(), al::CurrentDisplay.rect());


		{
			al::Transform t;
			t.translate(txtPos);
			al::ScopedTransform st(t);
			al::DrawLine({}, {txtMaxWidth,0.0f}, al::Red, 4.0);
			font.draw(
				txtTestCut,
				al::RGB(255,255,255), 
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
	};

	loop.eventDispatcher.setEventTypeHandler(
		ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 
		[&](const ALLEGRO_EVENT& ev){
			txtPos = {float(ev.mouse.x), float(ev.mouse.y)};
		}
	);

	auto keycodeDiscr = loop.eventDispatcher.addDiscretizer({
		ALLEGRO_EVENT_KEY_DOWN,
		[&](const ALLEGRO_EVENT& ev) {
			return ev.keyboard.keycode;
		}
	});

	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_UP, [&](const auto&) {
		txtTest = "UP was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_DOWN, [&](const auto&) {
		txtTest = "DOWN was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_LEFT, [&](const auto&) {
		txtTest = "LEFT was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_RIGHT, [&](const auto&) {
		txtTest = "RIGHT was pressed";
	});

	loop.enableFramerateLimit(165);
	loop.run();
}
