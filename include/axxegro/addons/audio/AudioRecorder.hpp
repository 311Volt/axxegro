#ifndef AXXEGRO_AUDIORECORDER_HPP
#define AXXEGRO_AUDIORECORDER_HPP

//TODO REMOVE THIS
#define ALLEGRO_UNSTABLE

#ifdef ALLEGRO_UNSTABLE
#include "AudioCommon.hpp"
#include "../../core/event/UserEvent.hpp"

namespace al {


	AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_RECORDER, al_destroy_audio_recorder);

	using AudioRecorderEvent = ALLEGRO_AUDIO_RECORDER_EVENT;

	template<>
	const AudioRecorderEvent& GetEventData<AudioRecorderEvent>(const Event& event) {
		return *(al_get_audio_recorder_event(const_cast<Event*>(&event)));
	}


	class BaseAudioRecorder: public Resource<ALLEGRO_AUDIO_RECORDER> {
	public:
		explicit BaseAudioRecorder(AudioFormat fmt = {}, int numBuffers = 16, int bufFragCount = 1024)
		: Resource(nullptr) {
			setPtr(al_create_audio_recorder(numBuffers, bufFragCount, fmt.frequency, fmt.depth, fmt.chanConf));
			if(!ptr()) {
				throw AudioError(
					"Cannot create audio recorder (requested config: %s format with %d*%d buffers)",
					fmt.str().c_str(), numBuffers, bufFragCount
				);
			}
		}

		bool start() {
			return al_start_audio_recorder(ptr());
		}

		void stop() {
			al_stop_audio_recorder(ptr());
		}

		bool isRecording() {
			return al_is_audio_recorder_recording(ptr());
		}



	private:

	};

	template<typename FragmentT>
	class AudioRecorder: public BaseAudioRecorder {
	public:

	private:

	};
}

#endif

#endif //AXXEGRO_AUDIORECORDER_HPP
