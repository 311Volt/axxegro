

#include <axxegro/axxegro.hpp>


int main()
{
	using namespace al::FreqLiterals;
	std::set_terminate(al::Terminate);
	al::Display disp(640, 480);
	al::EventLoop evLoop(al::DemoEventLoopConfig);
	al::Font font("data/roboto.ttf", 36);

	al::ReserveSamples(16);

	al::BaseAudioStream music = al::LoadAudioStream("data/audio/Spring_In_My_Step.ogg");
	al::DefaultMixer.attachAudioStream(music);
	music.setGain(0.6);
	music.setPlayMode(ALLEGRO_PLAYMODE_LOOP);

	al::Sample uuhhh = al::LoadSample("data/audio/uuhhh.ogg"); //sue me bitches

	evLoop.eventDispatcher.onKeyDown(ALLEGRO_KEY_F, [&](){
		uuhhh.play();
	});

	evLoop.framerateLimiter.setLimit(30_Hz);
	evLoop.run([&](){
		al::TargetBitmap.clearToColor(al::Blue);
		font.drawText("press F to oof", al::White, {100, 100});
		al::CurrentDisplay.flip();
	});
}
