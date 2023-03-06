#include <axxegro/axxegro.hpp>

#include <algorithm>
#include <string>
#include <queue>

/**
 * @file
 * 
 * An example on how to create and handle user events.
 */

//How to create custom user events in axxegro:
//First, we need an event type ID.
constexpr ALLEGRO_EVENT_TYPE AXX_EXAMPLE_EVENT = 1437;

//Then we define some data structure for the event.
struct ExampleEventData {
	float a,b;
	std::string msg;
};

//Finally, we associate one with the other using this template
using ExampleEventType = al::UserEventType<ExampleEventData, AXX_EXAMPLE_EVENT>;

/* We will emit user events using a user event source. The UserEventSource
 * class can be used standalone, but naturally you can extend it to keep some
 * state: */
class MyEventSource: public al::UserEventSource {
public:
	float state = 0.5;

	void fire()
	{
		ExampleEventData dat {state, 3.1, "this is some text"};
		this->emitEvent<ExampleEventType>(dat);
		state += 0.5;
	}
};


int main()
{
	al::FullInit();
	al::Display disp(640, 480);
	const auto& font = al::Font::BuiltinFont();

	//for displaying messages about events on screen
	std::deque<std::string> messages;

	//create the event loop
	auto evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();

	MyEventSource mySource;
	evLoop.eventQueue.registerSource(mySource);

	//this will be the handler for the event that we just defined
	evLoop.eventDispatcher.setEventTypeHandler(AXX_EXAMPLE_EVENT, [&](const ALLEGRO_EVENT& ev){

		const auto& dat = al::GetUserEventData<ExampleEventType>(ev);

		//reading the data we put in
		messages.push_back(al::Format(
			"[%.2f] Received example event: %.1f %.1f \"{}\"", 
			ev.any.timestamp, 
			dat.a, dat.b, dat.msg
		));

		//accessing the event source itself
		messages.push_back(al::Format(
			"Custom event source state: %f",
			static_cast<MyEventSource&>(al::GetUserEventSource(ev)).state
		));

		//delete old lines
		while(messages.size() >= 25) {
			messages.pop_front();
		}
	});

	evLoop.eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_KEY_CHAR, [&](const ALLEGRO_EVENT& ev){
		if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
			//ENTER has been pressed, let's emit the event
			mySource.fire();
		}
	});

	
	evLoop.loopBody = [&](){
		al::TargetBitmap.clearToColor(al::RGB(0,0,60));

		font.draw("Press ENTER to emit an event", al::White, {al::CurrentDisplay.width()/2.f, 10.f}, ALLEGRO_ALIGN_CENTER);

		int y = 50;
		for(auto& line: messages) {
			font.draw(line, al::White, {50.f, float(y)});
			y += font.getLineHeight() + 3;
		}

		al::CurrentDisplay.flip();
	};

	evLoop.enableFramerateLimit(60); //cap to 60Hz
	evLoop.run();
}
