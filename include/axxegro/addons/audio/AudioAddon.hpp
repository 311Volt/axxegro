//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_AUDIOADDON_HPP
#define AXXEGRO_AUDIOADDON_HPP

#include <allegro5/allegro_audio.h>

#include "../../com/Initializable.hpp"
#include "../../core/event/ExperimentalBuiltinEvents.hpp"

namespace al {

	inline Event2 TranslateAudioEvent(const AnyEvent& anyEv) {
		if(anyEv.type == ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT) {
			return AudioStreamChunkEvent {};
		}
		if(anyEv.type == ALLEGRO_EVENT_AUDIO_STREAM_FINISHED) {
			return AudioStreamFinishedEvent {};
		}
#ifdef ALLEGRO_UNSTABLE
		if(anyEv.type == ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT) {
			ALLEGRO_AUDIO_RECORDER_EVENT* recEv = al_get_audio_recorder_event(const_cast<ALLEGRO_EVENT*>(&anyEv));
			AudioRecorderChunkEvent ev {};
			ev.chunkLength = recEv->samples;
			ev.buffer = recEv->buffer;
			ev.recorder = nullptr;
			return ev;
		}
#endif
		throw EventError("TranslateAudioEvent called on event with type %d", anyEv.type);
	}

	struct AudioAddon {
		static constexpr char name[] = "Audio addon";
		[[nodiscard]] static bool isInitialized() {return al_is_audio_installed();}
		[[nodiscard]] static bool init() {

			detail::EventTranslators.push_back(detail::EventTranslator {
				.typeBegin = ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT,
#ifdef ALLEGRO_UNSTABLE
				.typeEnd = ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT,
#else
				.typeEnd = ALLEGRO_EVENT_AUDIO_STREAM_FINISHED,
#endif
				.translator = TranslateAudioEvent
			});

			return al_install_audio();
		}
		using DependsOn = InitDependencies<CoreAllegro>;
	};
}

#endif //AXXEGRO_AUDIOADDON_HPP
