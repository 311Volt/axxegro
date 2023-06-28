
#include <axxegro/axxegro.hpp>

int main()
{
	std::set_terminate(al::Terminate);

	al::Display disp(1024, 768);
	al::Video vid("data/cssd.ogv");
	al::Font builtinFont = al::Font::CreateBuiltinFont();

	al::SampleInstance::ReserveSamples(16);
	vid.start();

	al::EventQueue queue;
	queue.registerSource(al::GetKeyboardEventSource());
	queue.registerSource(vid.getEventSource());
	
	while(true) {
		al::TargetBitmap.clear();
		
		auto event = queue.wait();
		if(event.get().type == ALLEGRO_EVENT_VIDEO_FRAME_SHOW) {
			const auto* frame = vid.getFrame();
			frame->draw({0, 0});
			builtinFont.draw(al::Format("position: %.2f secs", vid.getPos()), al::PureGreen, frame->size());
		} else if(event.get().type == ALLEGRO_EVENT_VIDEO_FINISHED) {
			break;
		} else if(event.get().type == ALLEGRO_EVENT_KEY_DOWN) {
			if(event.get().keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				break;
			}
		}
		
		al::CurrentDisplay.flip();
	}
}
