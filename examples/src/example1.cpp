#include <axxegro/axxegro.hpp>

#include <allegro5/allegro.h>
#include <fmt/format.h>

#include <cmath>

int main()
{
	al::FullInit();
	al::Display disp(800, 600);
	al::Bitmap bg("data/bg.jpg");
	al::Font font("data/roboto.ttf", 24);

	al::EventLoop loop;
	loop.initDefaultEventQueue();
	loop.initDefaultDispatcher();

	al::Point txtPos {320, 240};
	std::string txtTest = fmt::format("kbstate size = {}, mstate size = {}", sizeof(ALLEGRO_KEYBOARD_STATE), sizeof(ALLEGRO_MOUSE_STATE));

	loop.loopBody = [&](){
		al::Display::Clear(al::Color::RGB(0,0,0));
		bg.draw({0, 0});
		font.draw(
			fmt::format("{}. tick={}", txtTest, loop.getTick()), 
			al::Color::RGB(255,255,255), 
			txtPos
		);
		al::Display::Flip();
	};

	loop.eventDispatcher.setEventTypeHandler(
		ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 
		[&](const ALLEGRO_EVENT& ev){
			txtPos = {ev.mouse.x, ev.mouse.y};
		}
	);

	auto keycodeDiscr = loop.eventDispatcher.addDiscretizer({
		ALLEGRO_EVENT_KEY_DOWN,
		[&](const ALLEGRO_EVENT& ev) {
			return ev.keyboard.keycode;
		}
	});

	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_UP, [&](const ALLEGRO_EVENT& ev) {
		txtTest = "UP was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_DOWN, [&](const ALLEGRO_EVENT& ev) {
		txtTest = "DOWN was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_LEFT, [&](const ALLEGRO_EVENT& ev) {
		txtTest = "LEFT was pressed";
	});
	loop.eventDispatcher.setEventValueHandler(keycodeDiscr, ALLEGRO_KEY_RIGHT, [&](const ALLEGRO_EVENT& ev) {
		txtTest = "RIGHT was pressed";
	});

	loop.enableClock(60);
	loop.run();

	return 0;
}