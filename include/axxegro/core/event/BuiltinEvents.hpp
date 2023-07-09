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

	using AnyEvent = ALLEGRO_ANY_EVENT;

	using DisplayEvent = ALLEGRO_DISPLAY_EVENT;
	using JoystickEvent = ALLEGRO_JOYSTICK_EVENT;
	using KeyboardEvent = ALLEGRO_KEYBOARD_EVENT;
	using MouseEvent = ALLEGRO_MOUSE_EVENT;
	using TimerEvent = ALLEGRO_TIMER_EVENT;
	using TouchEvent = ALLEGRO_TOUCH_EVENT;

	template<typename T>
	concept BuiltinEventType =
		std::same_as<T, DisplayEvent> ||
		std::same_as<T, JoystickEvent> ||
		std::same_as<T, KeyboardEvent> ||
		std::same_as<T, MouseEvent> ||
		std::same_as<T, TimerEvent> ||
		std::same_as<T, TouchEvent>;

	template<> struct EventDataGetter<DisplayEvent> {
		const DisplayEvent& operator()(const Event& ev) {return ev.display;}
	};

	template<> struct EventDataGetter<JoystickEvent> {
		const JoystickEvent& operator()(const Event& ev) {return ev.joystick;}
	};

	template<> struct EventDataGetter<KeyboardEvent> {
		const KeyboardEvent& operator()(const Event& ev) {return ev.keyboard;}
	};

	template<> struct EventDataGetter<MouseEvent> {
		const MouseEvent& operator()(const Event& ev) {return ev.mouse;}
	};

	template<> struct EventDataGetter<TimerEvent> {
		const TimerEvent& operator()(const Event& ev) {return ev.timer;}
	};

	template<> struct EventDataGetter<TouchEvent> {
		const TouchEvent& operator()(const Event& ev) {return ev.touch;}
	};

}

#endif //AXXEGRO_EVENT_EVENT_HPP
