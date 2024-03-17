#include <axxegro/axxegro.hpp>


int main() {
	al::Display disp(800, 600);

	al::Font font("data/roboto.ttf", 16);

	al::Timer frameTimer(al::Hz(60));

	al::EventQueue evQueue;
	evQueue.registerSource(al::GetKeyboardEventSource());
	evQueue.registerSource(al::GetMouseEventSource());
	evQueue.registerSource(al::CurrentDisplay.eventSource());

	bool exit = false;
	while(!exit) {
		auto event = evQueue.wait();
		al::VisitEvent(event.get(), al::Overloaded {
			[&](const al::DisplayCloseEvent&) {
				exit = true;
			},
			[&](const al::TimerEvent& ev) {
				al::CurrentDisplay.flip();
			}
		});
	}

}