
/*
 * An example on how to use axxegro's type-safe audio recorders & streams.
 */


/*
 * This #define is necessary to access al::AudioRecorder, as Allegro's audio
 * recorder API is still unstable.
 */
#define ALLEGRO_UNSTABLE
#include <axxegro/axxegro.hpp>

#include <queue>
#include <ranges>
#include <numbers>

static constexpr int AudioBufferSize = 1024;

/*
 * Utility
 */
float Peak(const std::span<float> samples)
{
	float ret = 0.0;
	for(auto& smp: samples) {
		ret = std::max(ret, std::fabs(smp));
	}
	return ret;
}

float dB(float val)
{
	return 20.0f * std::log10(val);
}

/*
 * This class keeps track of volume peaks.
 */
struct AudioMeter {

	void onBuffer(const std::span<al::Vec2f> buffer) {
		leftCh.resize(buffer.size());
		rightCh.resize(buffer.size());

		if(al::UnzipChannels(buffer, leftCh, rightCh)) {
			leftPeak = Peak(leftCh);
			rightPeak = Peak(rightCh);
		}
	}

	float leftPeak=0.0f, rightPeak=0.0f;

private:
	std::vector<float> leftCh, rightCh;
};


/*
 * In this example, we will be playing back the user's voice in a funny way.
 * This will be done with a ring modulator (we basically multiply everything by a sine wave)
 *
 * Note the span-based API, which allows the user to write allocation-free code.
 */
struct RingModulator {

	explicit RingModulator(double carrierFreq): buffer(10 * AudioBufferSize), carrierFrequency(carrierFreq) {}

	/*
	 * Take the input, process it and save it in the ring buffer.
	 */
	void consume(const std::span<al::Vec2f> input)
	{
		static std::vector<al::Vec2f> output;
		output.resize(input.size());

		for(unsigned i=0; i<input.size(); i++) {
			double tSecs = double(samplesProcessed++) / sampleRate;
			double carrier = std::sin(2.0 * std::numbers::pi * (tSecs * carrierFrequency));
			output[i] = input[i] * carrier;
		}

		buffer.pushData(output);
	}

	/*
	 * If able to, fill outputBuffer with processed samples.
	 */
	bool request(std::span<al::Vec2f> outputBuffer) {
		if(buffer.size() >= outputBuffer.size()) {
			buffer.popInto(outputBuffer);
			return true;
		}
		return false;
	}

private:
	al::RingBuffer<al::Vec2f> buffer;

	int64_t samplesProcessed = 0;
	double carrierFrequency = 440.0f;
	double sampleRate = 44100.0;

};


int main()
{
	al::Display disp(640, 480);
	std::set_terminate(al::Terminate);
	double playbackGain = 1.0;

	al::Font font("data/roboto.ttf", 16);

	/*
	 * The recorder. Defaults to 44100 Hz and 16 buffers 1024 fragments each.
	 *
	 * Note that AudioRecorder<float> is really just AudioRecorder<int16_t> under the hood
	 * but with automatic conversion provided by the handler created by createHandler().
	 * This is because floats are the most convenient by far for DSP and Allegro's recorder with FLOAT32
	 * depth is currently broken.
	 * This behavior is opt-out with AXXEGRO_USE_NATIVE_FLOAT32_AUDIO_RECORDER.
	 */
	al::AudioRecorder<float, al::Stereo> recorder(al::Hz(44100), {.fragmentsPerChunk = AudioBufferSize});


	/*
	 * The stream. Will be used to play back the user's voice to them.
	 */
	al::AudioStream<float, al::Stereo> stream(al::Hz(44100), {.fragmentsPerChunk = AudioBufferSize});


	//Set up a custom audio configuration
	al::Voice voice;
	al::UserMixer<float, al::Stereo> mixer;
	voice.attachMixer(mixer);
	mixer.attachAudioStream(stream);



	AudioMeter meter;
	RingModulator modulator(440);

	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();

	loop.eventQueue.registerSource(recorder.getEventSource());
	loop.eventQueue.registerSource(stream.getEventSource());

	/*
	 * The recorder will NOT generate any events unless started. You can query its status
	 * with the isRecording() method.
	 */
	recorder.start();

	/*
	 * The stream will also not generate any events unless it's set as "playing".
	 */
	stream.setPlaying(true);

	loop.eventDispatcher.setEventHandler(
		ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT,
		recorder.createHandler([&](const std::span<al::Vec2f> buffer) {

			// We just got audio data from the microphone, let's handle it

			modulator.consume(buffer);
			meter.onBuffer(buffer);
		})
	);

	loop.eventDispatcher.setEventHandler(
		ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT,
		stream.createChunkEventHandler([&](std::span<al::Vec2f> streamData) {

			/* The audio stream is ready for a new buffer. Let's ask our signal
			 * processor for audio data. */

			modulator.request(streamData);
		})
	);

	loop.eventDispatcher
		.onKeyCharKeycode(ALLEGRO_KEY_PAD_PLUS, [&](){playbackGain *= 1.1;})
		.onKeyCharKeycode(ALLEGRO_KEY_PAD_MINUS, [&](){playbackGain /= 1.1;});

	mixer.setPostprocessCallback([&](std::span<al::Vec2f> samples) {
		for(auto& smp: samples) {
			smp *= playbackGain;
		}
	});

	/*
	 * Note: The above event handlers will run in the main thread, which is also responsible
	 * for input handling and rendering. This is a bad idea: consider what happens if rendering
	 * is slow enough to not keep up with audio events.
	 *
	 * In your own programs, run a separate audio thread and keep separate event queues for audio
	 * so that you can respond to audio events immediately.
	 */

	al::RectI meterRectL = al::RectI::XYWH(20, 100, 400, 32);
	al::RectI meterRectR = meterRectL + al::Vec2i(0, 40);

	loop.loopBody = [&](){
		al::TargetBitmap.clear();

		/*
		 * Draw info text and peak meters
		 */
		auto msg = al::Format("peaks: L %02.1f dB | R %02.1f dB", dB(meter.leftPeak), dB(meter.rightPeak));
		font.drawText(msg, al::White, {50, 50});
		font.drawText(al::Format("playback gain: %02.1f dB", dB(playbackGain)), al::White, {50, 75});

		al::DrawFilledRectangle(meterRectL, al::RGB(20, 20, 20));
		al::DrawFilledRectangle(meterRectR, al::RGB(20, 20, 20));

		al::DrawFilledRectangle(meterRectL.scale({meter.leftPeak, 1}, {meterRectL.a.x, 0}), al::Green);
		al::DrawFilledRectangle(meterRectR.scale({meter.rightPeak, 1}, {meterRectR.a.x, 0}), al::Green);

		al::CurrentDisplay.flip();
	};

	loop.enableFramerateLimit(al::Hz(120));
	loop.run();

}