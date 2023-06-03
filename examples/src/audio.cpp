

#include <axxegro/axxegro.hpp>

int main()
{
	std::set_terminate(al::Terminate);
	al::Display disp(640, 480);
	al::EventLoop evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();
	al::Font font("data/roboto.ttf", 36);



	al::SampleInstance::ReserveSamples(16);

	al::AudioStream music("data/audio/Spring_In_My_Step.ogg");
	al::DefaultMixer.attachAudioStream(music);
	music.setGain(0.6);
	music.setPlayMode(ALLEGRO_PLAYMODE_LOOP);

	al::Sample uuhhh("data/audio/uuhhh.ogg"); //sue me bitches

	evLoop.loopBody = [&](){
		al::TargetBitmap.clearToColor(al::Blue);
		font.draw("press F to oof", al::White, {100, 100});
		al::CurrentDisplay.flip();
	};

	evLoop.eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_KEY_DOWN, [&](const ALLEGRO_EVENT& ev) {
		if(ev.keyboard.keycode == ALLEGRO_KEY_F) {
			uuhhh.play();
		}
	});

	evLoop.enableFramerateLimit(30);
	evLoop.run();
}
