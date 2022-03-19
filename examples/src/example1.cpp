#include <axxegro/axxegro.hpp>

#include <allegro5/allegro.h>
#include <fmt/format.h>

#include <cmath>

int main()
{
	al::FullInit();
	al::Display disp(800, 600);
	al::Bitmap bg("xp.jpg");
	al::Font fn("roboto.ttf", 24);

	al::EventQueue eqIn;
	al::EventQueue eqClk;
	al::Timer clk = al::Timer::Freq(60);
	eqIn.registerSource(al::mouse::GetEventSource());
	eqIn.registerSource(al::keyb::GetEventSource());
	eqClk.registerSource(clk.getEventSource());
	
	al::Point txtPos {320, 240};

	while(1) {
		while(!eqClk.empty())
			eqClk.wait();
		ALLEGRO_EVENT ev = eqIn.wait();
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				return 0;
			}
		} else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			txtPos = {ev.mouse.x, ev.mouse.y};
		}
		
		al::Display::Clear(al::Color::RGB(0,0,0));
		bg.draw({0, 0});
		fn.draw(fmt::format("This is a test. tick={}", clk.getCount()), al::Color::RGB(255,255,255), txtPos);
		al::Display::Flip();
	}

	return 0;
}