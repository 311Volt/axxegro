
#include <axxegro/axxegro.hpp>

int main()
{
	al::FullInit();
	std::set_terminate(al::Terminate);

	al::Display disp(1024, 768);
	al::Video vid("data/cssd.ogv");
	al::Font builtinFont = al::Font::CreateBuiltinFont();

	al::SampleInstance::ReserveSamples(16);

	vid.start();

	auto evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();
	evLoop.loopBody = [&](){
		al::TargetBitmap.clear();

		const al::Bitmap* frm = vid.getFrame();
		if(frm) {
			frm->draw({0, 0});
		}
		builtinFont.draw(al::Format("position: %.2f secs", vid.getPos()), al::PureGreen, {500, 600});

		if(!vid.isPlaying()) {
			evLoop.setExitFlag();
		}

		al_flip_display();
	};
	evLoop.enableFramerateLimit(30);

	evLoop.run();
}
