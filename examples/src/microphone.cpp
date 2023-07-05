//
// Created by volt on 2023-07-02.
//

#define ALLEGRO_UNSTABLE
#include <axxegro/axxegro.hpp>

int16_t peakVolume(const std::span<int16_t> samples)
{
	float max = 0.0;
	for(auto& smp: samples) {
		max = std::max<float>(max, std::abs(smp));
	}
	return max;
}


int main()
{
	al::Display disp(800, 600);
	std::set_terminate(al::Terminate);

	al::Font font = al::Font::CreateBuiltinFont();

	al::BaseAudioRecorder dup({
		.depth = ALLEGRO_AUDIO_DEPTH_INT16,
		.chanConf = ALLEGRO_CHANNEL_CONF_1
	});

	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();

	float peakVol = 0;
	int numEvents = 0;

	loop.eventQueue.registerSource(al_get_audio_recorder_event_source(dup.ptr()));
	dup.start();

	loop.eventDispatcher.setEventHandler<al::AudioRecorderEvent>(ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT, [&](const al::AudioRecorderEvent& event){
		numEvents++;
		std::span<int16_t> data((int16_t*)event.buffer, (int16_t*)event.buffer + event.samples);
		peakVol = peakVolume(data);
	});



	loop.loopBody = [&](){
		al::TargetBitmap.clear();

		font.draw(al::Format("[%05d] volume: %.6f", numEvents, peakVol), al::White, {100, 100});

		al::CurrentDisplay.flip();
	};

	loop.enableFramerateLimit(al::Hz(30));
	loop.run();

}