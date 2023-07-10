
/*
 * An example on how to use axxegro's type-safe audio recorder.
 */


/*
 * This #define is necessary to access al::AudioRecorder, as Allegro's audio
 * recorder API is still unstable.
 */
#define ALLEGRO_UNSTABLE
#include <axxegro/axxegro.hpp>

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


int main()
{
	al::Display disp(640, 480);
	std::set_terminate(al::Terminate);

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
	al::AudioRecorder<float, al::Stereo> recorder;

	AudioMeter meter;

	al::EventLoop loop = al::EventLoop::Basic();
	loop.enableEscToQuit();

	loop.eventQueue.registerSource(recorder.getEventSource());

	/*
	 * The recorder will NOT generate any events unless started. You can query its status
	 * with the isRecording() method.
	 */
	recorder.start();


	loop.eventDispatcher.setEventHandler(
		ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT,
		recorder.createHandler([&](const std::span<al::Vec2f> buffer) {
			meter.onBuffer(buffer);
		})
	);

	al::RectI meterRectL = al::RectI::XYWH(20, 100, 400, 32);
	al::RectI meterRectR = meterRectL + al::Vec2i(0, 40);

	loop.loopBody = [&](){
		al::TargetBitmap.clear();

		/*
		 * Draw info text and peak meters
		 */
		auto msg = al::Format("peaks: L %02.1f dB | R %02.1f dB", dB(meter.leftPeak), dB(meter.rightPeak));
		font.drawText(msg, al::White, {50, 50});

		al::DrawFilledRectangle(meterRectL, al::RGB(20, 20, 20));
		al::DrawFilledRectangle(meterRectR, al::RGB(20, 20, 20));

		al::DrawFilledRectangle(meterRectL.scale({meter.leftPeak, 1}, {meterRectL.a.x, 0}), al::Green);
		al::DrawFilledRectangle(meterRectR.scale({meter.rightPeak, 1}, {meterRectR.a.x, 0}), al::Green);

		al::CurrentDisplay.flip();
	};

	loop.enableFramerateLimit(al::Hz(60));
	loop.run();

}