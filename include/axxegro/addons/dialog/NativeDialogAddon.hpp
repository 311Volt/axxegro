//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_NATIVEDIALOGADDON_HPP
#define AXXEGRO_NATIVEDIALOGADDON_HPP

#include "../../com/Initializable.hpp"
#include "../../core/event/ExperimentalBuiltinEvents.hpp"
#include <allegro5/allegro_native_dialog.h>

namespace al {

	inline Event2 TranslateNativeDialogEvent(const AnyEvent& anyEv) {
		if(anyEv.type == ALLEGRO_EVENT_NATIVE_DIALOG_CLOSE) {
			NativeDialogCloseEvent ev {};
			ev.log = nullptr;
			ev.wasClosedByKeyPress = anyEv.user.data2;
			return ev;
		}
		if(anyEv.type == ALLEGRO_EVENT_MENU_CLICK) {
			MenuClickEvent ev {};
			ev.itemId = anyEv.user.data1;
			ev.display = nullptr; //data2
			ev.menu = nullptr; //data3
			return ev;
		}
		throw EventError("TranslateNativeDialogEvent called on an event with invalid type %d", anyEv.type);
	}

	struct NativeDialogAddon {
		static constexpr char name[] = "Native dialog addon";
		[[nodiscard]] static bool isInitialized() {return al_is_native_dialog_addon_initialized();}
		[[nodiscard]] static bool init() {
			detail::EventTranslators.push_back(detail::EventTranslator {
				.typeBegin = ALLEGRO_EVENT_NATIVE_DIALOG_CLOSE,
				.typeEnd = ALLEGRO_EVENT_MENU_CLICK,
				.translator = TranslateNativeDialogEvent
			});
			return al_init_native_dialog_addon();
		}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

}

#endif //AXXEGRO_NATIVEDIALOGADDON_HPP
