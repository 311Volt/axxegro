#include <axxegro/axxegro.hpp>
#include <axxegro/core/event/EventDispatcher.hpp>

#include <queue>

/**
 * @file
 * 
 * An example on how to create and handle user events.
 */

//How to create custom user events in axxegro:

//Define a data structure for the event.
struct ExampleEvent {
	/* You may optionally let axxegro know its type ID here, like this:
	 *		static constexpr al::EventType EventTypeID = 1437;
	 * This must be a value between 1024 and (1<<30) and will exclude this type
	 * from event type ID auto-generation.
	 */

	float a,b;
	std::string msg;
};

/*
 * Another event. This one is trivially copyable and small enough to fit in
 * the size of 4 pointers, so axxegro will be smart enough to store it in the
 * ev.user.data1-data4 fields of the ALLEGRO_USER_EVENT struct, no additional
 * allocation needed.
 */
struct SimpleEvent {
	int a, b;
};


/*
 * We will emit user events using a user event source. The UserEventSource
 * class can be used standalone, but naturally you can extend it to keep some
 * state:
 */
class MyEventSource: public al::UserEventSource {
public:
	float state = 0.5;

	void customEmitEvent()
	{
		ExampleEvent dat {state, 3.1, "this is some text"};
		this->emitEvent(dat);
		state += 0.5;
	}
};


int main()
{

	al::Display disp(640, 480);
	const auto& font = al::Font::BuiltinFont();

	//for displaying messages about events on screen
	std::deque<std::string> messages;

	//create the event loop
	al::EventLoop evLoop(al::DemoEventLoopConfig);


	MyEventSource mySource;
	evLoop.eventQueue.registerSource(mySource);

	//a handler for ExampleEvent
	evLoop.eventDispatcher.setUserEventHandler<ExampleEvent>([&](const ExampleEvent& ev, const al::AnyEvent& meta){

		//reading the data we put in
		messages.push_back(al::Format(
			"[%.2f] Received example event (id=%d): %.1f %.1f \"%s\"",
			meta.timestamp, meta.type,
			ev.a, ev.b, ev.msg.c_str()
		));

		//accessing the event source itself
		messages.push_back(al::Format(
			"Custom event source state: %f",
			dynamic_cast<MyEventSource&>(al::GetUserEventSource(meta)).state
		));
	});

	//a handler for the simple event. note that you can skip some parameters in the lambda
	evLoop.eventDispatcher.setUserEventHandler<SimpleEvent>([&](const SimpleEvent& ev){
		messages.push_back(al::Format("simple event received: %d, %d", ev.a, ev.b));
	});

	
	evLoop.eventDispatcher
		.onKeyCharKeycode(ALLEGRO_KEY_A, [&](){mySource.customEmitEvent();})
		.onKeyCharKeycode(ALLEGRO_KEY_Z, [&](){mySource.emitEvent(SimpleEvent{.a = 1, .b = 2});});

	evLoop.run([&](){
		al::TargetBitmap.clearToColor(al::RGB(0,0,60));

		font.drawText("Press A or Z to emit events", al::White, {al::CurrentDisplay.width() / 2.f, 10.f},
					  ALLEGRO_ALIGN_CENTER);

		//delete old lines from log
		while(messages.size() >= 25) {
			messages.pop_front();
		}

		int y = 50;
		for(auto& line: messages) {
			font.drawText(line, al::White, {50.f, float(y)});
			y += font.getLineHeight() + 3;
		}


		al::CurrentDisplay.flip();
	});
}
