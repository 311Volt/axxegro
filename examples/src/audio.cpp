

#include <axxegro/axxegro.hpp>

int main()
{
	al::FullInit();
	std::set_terminate(al::Terminate);

	al::Display disp(640, 480);
	al::EventLoop evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();

	al::Font font("data/roboto.ttf", 36);

	al::SampleInstance::ReserveSamples(16);
	al::Sample uuhhh("data/audio/uuhhh.ogg"); //sue me bitches

	evLoop.loopBody = [&](){
		al::TargetBitmap.clearToColor(al::Blue);
		font.draw("press enter to oof", al::White, {100, 100});
		al::CurrentDisplay.flip();
	};

	evLoop.eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_KEY_DOWN, [&](const ALLEGRO_EVENT& ev) {
		if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
			uuhhh.play();
		}
	});

	evLoop.enableFramerateLimit(30);
	evLoop.run();
}
