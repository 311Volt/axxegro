//
// Created by volt on 2023-06-26.
//

#ifndef AXXEGRO_EVENT_EVENT_HPP
#define AXXEGRO_EVENT_EVENT_HPP

#include <allegro5/allegro.h>
#include <concepts>
#include "EventDataGetter.hpp"

namespace al {

	using Event = ALLEGRO_EVENT;
	using EventType = ALLEGRO_EVENT_TYPE;

	using EventInfo = ALLEGRO_ANY_EVENT;

	using LegacyDisplayEvent = ALLEGRO_DISPLAY_EVENT;
	using LegacyJoystickEvent = ALLEGRO_JOYSTICK_EVENT;
	using LegacyKeyboardEvent = ALLEGRO_KEYBOARD_EVENT;
	using LegacyMouseEvent = ALLEGRO_MOUSE_EVENT;
	using LegacyTimerEvent = ALLEGRO_TIMER_EVENT;
	using LegacyTouchEvent = ALLEGRO_TOUCH_EVENT;

	template<typename T>
	concept BuiltinEventType =
		std::same_as<T, LegacyDisplayEvent> ||
		std::same_as<T, LegacyJoystickEvent> ||
		std::same_as<T, LegacyKeyboardEvent> ||
		std::same_as<T, LegacyMouseEvent> ||
		std::same_as<T, LegacyTimerEvent> ||
		std::same_as<T, LegacyTouchEvent>;

	template<> struct EventDataGetter<LegacyDisplayEvent> {
		const LegacyDisplayEvent& operator()(const Event& ev) {return ev.display;}
	};

	template<> struct EventDataGetter<LegacyJoystickEvent> {
		const LegacyJoystickEvent& operator()(const Event& ev) {return ev.joystick;}
	};

	template<> struct EventDataGetter<LegacyKeyboardEvent> {
		const LegacyKeyboardEvent& operator()(const Event& ev) {return ev.keyboard;}
	};

	template<> struct EventDataGetter<LegacyMouseEvent> {
		const LegacyMouseEvent& operator()(const Event& ev) {return ev.mouse;}
	};

	template<> struct EventDataGetter<LegacyTimerEvent> {
		const LegacyTimerEvent& operator()(const Event& ev) {return ev.timer;}
	};

	template<> struct EventDataGetter<LegacyTouchEvent> {
		const LegacyTouchEvent& operator()(const Event& ev) {return ev.touch;}
	};

}

#endif //AXXEGRO_EVENT_EVENT_HPP
