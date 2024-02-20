//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_VIDEOADDON_HPP
#define AXXEGRO_VIDEOADDON_HPP

#include <allegro5/allegro_video.h>
#include "../../com/Initializable.hpp"
#include "../audio/AudioAddon.hpp"
#include "../../core/event/ExperimentalBuiltinEvents.hpp"

namespace al {

	inline Event2 TranslateVideoAddonEvent(const AnyEvent& anyEv) {
		if(anyEv.type == ALLEGRO_EVENT_VIDEO_FRAME_SHOW) {
			VideoFrameShowEvent ev {};
			ev.video = nullptr;
			return ev;
		}
		if(anyEv.type == ALLEGRO_EVENT_VIDEO_FINISHED) {
			VideoFinishedEvent ev {};
			ev.video = nullptr;
			return ev;
		}
		throw EventError("TranslateVideoAddonEvent called on event with invalid type %d", anyEv.type);
	}

	struct VideoAddon {
		static constexpr char name[] = "Video addon";
		[[nodiscard]] static bool isInitialized() {return al_is_video_addon_initialized();}
		[[nodiscard]] static bool init() {
			detail::EventTranslators.push_back(detail::EventTranslator{
				.typeBegin = ALLEGRO_EVENT_VIDEO_FRAME_SHOW,
				.typeEnd = ALLEGRO_EVENT_VIDEO_FINISHED,
				.translator = TranslateVideoAddonEvent
			});
			return al_init_video_addon();
		}
		using DependsOn = InitDependencies<AudioAddon>;
	};
}

#endif //AXXEGRO_VIDEOADDON_HPP
