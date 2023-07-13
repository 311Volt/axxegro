

#include <axxegro/axxegro.hpp>


int main()
{
	using namespace al::FreqLiterals;
	std::set_terminate(al::Terminate);
	al::Display disp(640, 480);
	al::EventLoop evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();
	al::Font font("data/roboto.ttf", 36);



	al::SampleInstance::ReserveSamples(16);

	al::BaseAudioStream music("data/audio/Spring_In_My_Step.ogg");
	al::DefaultMixer.attachAudioStream(music);
	music.setGain(0.6);
	music.setPlayMode(ALLEGRO_PLAYMODE_LOOP);

	al::Sample uuhhh("data/audio/uuhhh.ogg"); //sue me bitches

	evLoop.loopBody = [&](){
		al::TargetBitmap.clearToColor(al::Blue);
		font.drawText("press F to oof", al::White, {100, 100});
		al::CurrentDisplay.flip();
	};

	evLoop.eventDispatcher.onKeyDown(ALLEGRO_KEY_F, [&](){
		uuhhh.play();
	});

	evLoop.enableFramerateLimit(30_Hz);
	evLoop.run();
}
