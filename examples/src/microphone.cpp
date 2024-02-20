
/*
 * An example on how to use axxegro's type-safe audio recorders & streams.
 */


/*
 * This #define is necessary to access al::AudioRecorder, as Allegro's audio
 * recorder API is still unstable.
 */
#define ALLEGRO_UNSTABLE
#include <axxegro/axxegro.hpp>
#include <axxegro/addons/audio/AudioAddon.hpp>

#include <queue>
#include <numbers>
#include <thread>
#include <atomic>

using namespace al::Literals;

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

	void consume(const std::span<const al::Vec2f> buffer) {
		leftCh.resize(buffer.size());
		rightCh.resize(buffer.size());

		if(al::UnzipChannels(buffer, leftCh, rightCh)) {
			leftPeak = Peak(leftCh);
			rightPeak = Peak(rightCh);
		}
	}

	std::atomic<float> leftPeak=0.0f, rightPeak=0.0f;

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

	explicit RingModulator(al::Hz carrierFreq): buffer(32768), carrierFrequency(carrierFreq) {}

	/*
	 * Take the input, process it and save it in the ring buffer.
	 */
	bool consume(const std::span<const al::Vec2f> input)
	{
		static std::vector<al::Vec2f> output;
		output.resize(input.size());

		for(unsigned i=0; i<input.size(); i++) {
			double tSecs = double(samplesProcessed++) / sampleRate;
			double carrier = std::sin(2.0 * std::numbers::pi * (tSecs * carrierFrequency.getHz()));
			output[i] = input[i] * carrier;
			//output[i] = al::Vec2f(0.2, 0.2) * carrier;
		}

		return buffer.pushData(output);
	}

	/*
	 * If able to, fill outputBuffer with processed samples.
	 */
	bool request(std::span<al::Vec2f> outputBuffer) {
		if(buffer.size() < cooldown) {
			return false;
		}

		cooldown = 0;
		if(buffer.popInto(outputBuffer)) {
			return true;
		} else {
			// Delay the stream a bit to ensure smooth playback
			cooldown = 2 * outputBuffer.size();
			return false;
		}
	}

	int bufsize() {return buffer.size();}
private:
	al::RingBuffer<al::Vec2f> buffer;

	int64_t samplesProcessed = 0;
	size_t cooldown = 0;
	al::Hz carrierFrequency;
	double sampleRate = 44100.0;

};

/*
 * This class contains our audio code.
 *
 * While axxegro's audio streams are technically usable without creating
 * a separate thread, every serious application should have its own
 * audio thread. This way, we can handle audio chunk events as soon
 * as they happen.
 */
class AudioThreadRunnable {
public:

	al::Voice voice;
	al::UserMixer<float, al::Stereo> mixer;


	/*
	 * The stream. Will be used to play back the user's voice to them.
	 */
	al::AudioStream<float, al::Stereo> stream;

	/*
	 * The recorder. Defaults to 44100 Hz and 16 buffers 1024 fragments each.
	 *
	 * Note that AudioRecorder<float> is really just AudioRecorder<int16_t> under the hood
	 * but with automatic conversion provided by the handler created by createHandler().
	 * This is because floats are the most convenient by far for DSP and Allegro's recorder with FLOAT32
	 * depth is currently broken.
	 * This behavior is opt-out with AXXEGRO_USE_NATIVE_FLOAT32_AUDIO_RECORDER.
	 */
	al::AudioRecorder<float, al::Stereo> recorder;

	al::EventDispatcher dispatcher;

	AudioMeter meter;
	RingModulator modulator;

	std::atomic<double> playbackGain = 1.0;

	AudioThreadRunnable(unsigned streamBufSize, unsigned recBufSize)
		: mixer(44100_Hz),
		  stream(44100_Hz, {.numChunks = 3, .fragmentsPerChunk = streamBufSize}),
		  recorder(44100_Hz, {.numChunks = 12, .fragmentsPerChunk = recBufSize}),
		  modulator(440_Hz)
	{

		voice.attachMixer(mixer);
		mixer.attachAudioStream(stream);

		dispatcher.setEventHandler(
			ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT,
			recorder.createChunkEventHandler(
				[&](const std::span<const al::Vec2f> buffer) {

					// We just got audio data from the microphone, let's handle it
					modulator.consume(buffer);
					meter.consume(buffer);
				}
			)
		);

		dispatcher.setEventHandler(
			ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT,
			stream.createChunkEventHandler([&](std::span<al::Vec2f> streamData) {

				/* The audio stream is ready for a new buffer. Let's ask our signal
				 * processor for audio data. */
				modulator.request(streamData);
			})
		);

		mixer.setPostprocessCallback([&](std::span<al::Vec2f> buf) {
			auto gain = float(playbackGain);
			for(auto& frag: buf) {
				frag *= gain;
			}
		});


	}


	void run(const std::stop_token& token) {
		al::EventQueue queue;
		queue.registerSource(stream.getEventSource());
		queue.registerSource(recorder.getEventSource());

		stream.setPlaying(true);
		recorder.start();

		while(!token.stop_requested()) {
			if(auto event = queue.waitFor(0.5)) {
				dispatcher.dispatch(event->get());
			}
		}

		stream.setPlaying(false);
		recorder.stop();

	}

	void mulGain(double factor) {
		playbackGain = playbackGain * factor;
	}


};


int main()
{
	al::Display disp(640, 480);
	std::set_terminate(al::Terminate);

	al::Font font("data/roboto.ttf", 16);

	AudioThreadRunnable audio(1024, 1024);
	std::jthread audioThread([&](std::stop_token tok){audio.run(tok);});

	al::EventLoop loop(al::DemoEventLoopConfig);


	loop.eventDispatcher
		.onKeyCharKeycode(ALLEGRO_KEY_PAD_PLUS, [&](){audio.mulGain(1.1);})
		.onKeyCharKeycode(ALLEGRO_KEY_PAD_MINUS, [&](){audio.mulGain(1.0 / 1.1);});


	al::RectI meterRectL = al::RectI::XYWH(20, 100, 400, 32);
	al::RectI meterRectR = meterRectL + al::Vec2i(0, 40);

	loop.run([&](){
		al::TargetBitmap.clear();

		float peakL = audio.meter.leftPeak;
		float peakR = audio.meter.rightPeak;

		/*
		 * Draw info text and peak meters
		 */
		auto msg = al::Format("peaks: L %02.1f dB | R %02.1f dB", dB(peakL), dB(peakR));
		font.drawText(msg, al::White, {50, 50});
		font.drawText(al::Format("playback gain: %02.1f dB", dB(audio.playbackGain)), al::White, {50, 75});

		al::DrawFilledRectangle(meterRectL, 0x141414_RGB);
		al::DrawFilledRectangle(meterRectR, 0x141414_RGB);

		al::DrawFilledRectangle(meterRectL.scale({peakL, 1}, {meterRectL.a.x, 0}), al::Green);
		al::DrawFilledRectangle(meterRectR.scale({peakR, 1}, {meterRectR.a.x, 0}), al::Green);

		al::CurrentDisplay.flip();
	});

}