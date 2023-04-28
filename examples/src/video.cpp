
#include <axxegro/axxegro.hpp>

int main()
{
	al::FullInit();
	std::set_terminate(al::Terminate);

	al::Display disp(1024, 768);
	al::Video vid("data/cssd.ogv");
	puts("vid loaded");
	al::Font builtinFont = al::Font::CreateBuiltinFont();

	al::SampleInstance::ReserveSamples(16);

	vid.start();
	puts("vid started");

	auto evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();
	evLoop.loopBody = [&](){
		printf("getting frame from video\n");
		//const al::Bitmap* frm = vid.getFrame();
		ALLEGRO_BITMAP* frm = al_get_video_frame(vid.ptr());
		if(frm) {
//			printf("drawing frame %dx%d\n", frm->width(), frm->height());
//			frm->draw({0, 0});
			printf("drawing frame %dx%d\n", al_get_bitmap_width(frm), al_get_bitmap_height(frm));
			al_draw_bitmap(frm, 0, 0, 0);
		}
		builtinFont.draw(al::Format("position: %.2f secs", vid.getPos()), al::PureGreen, {500, 400});

		if(!vid.isPlaying()) {
			evLoop.setExitFlag();
		}

		al_flip_display();
	};
	evLoop.enableFramerateLimit(30);

	evLoop.run();
}
