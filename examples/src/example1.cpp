#include <axxegro/axxegro.hpp>

#include <fmt/format.h>

#include <cmath>

/**
 * @file
 * The file where I test random things I implement. I guess it can showcase some
 * axxegro features, but for clearer and better explained examples, see other
 * files from this folder.
 */

int main()
{
	al::FullInit();
	al::Display disp(1024, 768, ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	al::Bitmap::SetNewBitmapFlags(ALLEGRO_MIPMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al::Bitmap bg("data/bg.jpg");
	al::Font font("data/roboto.ttf", 24);

	al::Config cfg("data/samplecfg.ini");

	for(auto& section: cfg.sections()) {
		fmt::print("{}: \n", section);
		for(auto& key: cfg.keys(section)) {
			al::Config::SectionSelector ss(cfg, section);
			fmt::print("\t{} -> {}\n", key, cfg.getValue(key));
		}
		fmt::print("\n\n");
	}


	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();

	al::Coord<float> txtPos {320, 240};
	std::string txtTest = fmt::format("kb {}B, m {}B", sizeof(ALLEGRO_KEYBOARD_STATE), sizeof(ALLEGRO_MOUSE_STATE));
	loop.loopBody = [&](){
		al::CurrentDisplay().clearToColor(al::Color::RGB(0,0,0));

		float txtMaxWidth = 10.0 + (0.5+0.5*std::sin(al::GetTime())) * 300.0;
		std::string txtTest1 = fmt::format("{}. tick={}", txtTest, loop.getTick());
		std::string txtTestCut = txtTest1.substr(0, font.calcCutoffPoint(txtTest1, txtMaxWidth));

		{
			int y = loop.getTick() % (bg.height()-10);
			int x = loop.getTick() % (bg.width()-10);
			al::Coord<int> p{x, y}, bb{2, 2};
			al::BitmapLockedRegion lr(bg, {p, p+bb}, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READWRITE);
			lr.rowData(1)[1] = 113;
		}
		

		bg.drawScaled(bg.rect(), al::CurrentDisplay().rect());


		{
			al::Transform t;
			t.translate(txtPos);
			al::ScopedTransform st(t);
			al::DrawLine({}, {txtMaxWidth,0.0f}, al::Col::Red, 4.0);
			font.draw(
				txtTestCut,
				al::Color::RGB(255,255,255), 
				{0, 0}
			);
		}
		
		al::CurrentDisplay().flip();
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

	loop.enableClock(165);
	loop.run();
}