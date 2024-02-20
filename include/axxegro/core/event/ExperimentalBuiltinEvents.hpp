
#ifndef AXXEGRO_EXPERIMENTALBUILTINEVENTS_HPP
#define AXXEGRO_EXPERIMENTALBUILTINEVENTS_HPP

#include <allegro5/allegro.h>
#include <variant>
#include <span>

#include "../../com/util/General.hpp"
#include "../../com/math/math.hpp"
#include "../../core/io/Keyboard.hpp"
#include "../../core/io/Mouse.hpp"

namespace al {

	class EventSource;
	class Joystick;
	class Display;
	class Timer;
	class Bitmap;
	class BaseAudioRecorder;
	class TextLog;
	class Video;
	class Menu;


	namespace detail::MouseEventMixins {
		struct MousePosition {
			Vec2i pos;
			int mWheelPos;
		};
		struct MouseDelta {
			Vec2i delta;
			int mWheelDelta;
		};
		struct MousePressure {
			float pressure;
		};
	}


	inline namespace Events {

		inline namespace JoystickEvents {
			struct JoystickEvent {
				Joystick* joystick;
			};

			struct JoystickAxisEvent: JoystickEvent {
				int stick;
				int axis;
				float pos;
			};

			struct JoystickButtonEvent: JoystickEvent {
				int button;
			};

			struct JoystickButtonDownEvent: JoystickButtonEvent {};
			struct JoystickButtonUpEvent: JoystickButtonEvent {};
		}

		inline namespace KeyboardEvents {

			struct KeyboardEvent {
				Display* sourceDisplay;
			};

			struct KeyDownEvent: KeyboardEvent {
				Key keycode;
			};

			struct KeyUpEvent: KeyboardEvent {
				Key keycode;
			};

			struct KeyCharEvent: KeyboardEvent {
				Key keycode;
				char32_t unichar;
				unsigned modifiers;
				bool repeat;

				[[nodiscard]] bool wasShiftDown() const {
					return modifiers & ALLEGRO_KEYMOD_SHIFT;
				}
				[[nodiscard]] bool wasCtrlDown() const {
					return modifiers & ALLEGRO_KEYMOD_CTRL;
				}
				[[nodiscard]] bool wasAltDown() const {
					return modifiers & ALLEGRO_KEYMOD_ALT;
				}
			};

		}

		inline namespace MouseEvents {
			struct MouseEvent {
				Display* sourceDisplay;
			};

			namespace mx = detail::MouseEventMixins;


			struct GenericMouseAxesEvent
				: MouseEvent, mx::MousePosition, mx::MouseDelta, mx::MousePressure
			{};

			struct GenericMouseButtonEvent
				: MouseEvent, mx::MousePosition, mx::MousePressure
			{
				unsigned button;
			};

			struct MouseEnterOrLeaveDisplayEvent
				: MouseEvent, mx::MousePosition
			{};

			struct MouseMovedEvent: GenericMouseAxesEvent {};
			struct MouseWarpedEvent: GenericMouseAxesEvent {};

			struct MouseButtonDownEvent: GenericMouseButtonEvent {};
			struct MouseButtonUpEvent: GenericMouseButtonEvent {};

			struct LMBDownEvent: MouseButtonDownEvent {};
			struct LMBUpEvent: MouseButtonUpEvent {};
			struct RMBDownEvent: MouseButtonDownEvent {};
			struct RMBUpEvent: MouseButtonUpEvent {};

			struct MouseEnterDisplayEvent: MouseEnterOrLeaveDisplayEvent {};
			struct MouseLeaveDisplayEvent: MouseEnterOrLeaveDisplayEvent {};
		}

		inline namespace TouchEvents {
			struct TouchEvent {
				Display* sourceDisplay;
				int id;
				Vec2f pos;
				Vec2f speed;
				bool primary;
			};

			struct TouchBeginEvent: TouchEvent {};
			struct TouchMoveEvent: TouchEvent {};
			struct TouchEndEvent: TouchEvent {};
			struct TouchCancelEvent: TouchEvent {};
		}

		struct TimerEvent {
			Timer* timer;
			int64_t count;
		};

		inline namespace DisplayEvents {
			struct DisplayEvent {
				Display* display;
			};

			struct DisplayExposeEvent: DisplayEvent {
				RectI exposedRect;
			};

			struct DisplayResizeEvent: DisplayEvent {
				Vec2i pos;
				Vec2i newSize;

				[[nodiscard]] RectI rect() const {
					return {pos, newSize};
				}
			};

			struct DisplayCloseEvent: DisplayEvent {};
			struct DisplayLostEvent: DisplayEvent {};
			struct DisplayFoundEvent: DisplayEvent {};
			struct DisplaySwitchOutEvent: DisplayEvent {};
			struct DisplaySwitchInEvent: DisplayEvent {};

			struct DisplayHaltDrawingEvent: DisplayEvent {};
			struct DisplayResumeDrawingEvent: DisplayEvent {};

			struct DisplayConnectedEvent: DisplayEvent {};
			struct DisplayDisconnectedEvent: DisplayEvent {};

			struct DisplayOrientationEvent: DisplayEvent {
				int orientation;
			};
		}

		inline namespace AudioEvents {

			struct AudioStreamChunkEvent {};
			struct AudioStreamFinishedEvent {};

			struct AudioRecorderChunkEvent {
				BaseAudioRecorder* recorder;
				void* buffer;
				unsigned chunkLength;
			};
		}

		inline namespace NativeDialogEvents {
			struct NativeDialogCloseEvent {
				TextLog* log;
				bool wasClosedByKeyPress;
			};

			struct MenuClickEvent {
				Display* display;
				Menu* menu;
				unsigned itemId;
			};
		}

		inline namespace VideoEvents {
			struct VideoFrameShowEvent {
				Video* video;
			};

			struct VideoFinishedEvent {
				Video* video;
			};
		}

		struct UserEvent {
			std::array<intptr_t, 4> data;
		};
	}

	using AnyEvent = ALLEGRO_EVENT;

	using Event2 = std::variant<
		JoystickAxisEvent,
		JoystickButtonEvent,
		JoystickButtonDownEvent,
		JoystickButtonUpEvent,
		KeyDownEvent,
		KeyUpEvent,
		KeyCharEvent,
		GenericMouseAxesEvent,
		GenericMouseButtonEvent,
		MouseEnterOrLeaveDisplayEvent,
		MouseMovedEvent,
		MouseWarpedEvent,
		MouseButtonDownEvent,
		MouseButtonUpEvent,
		LMBDownEvent,
		LMBUpEvent,
		RMBDownEvent,
		RMBUpEvent,
		MouseEnterDisplayEvent,
		MouseLeaveDisplayEvent,
		TouchBeginEvent,
		TouchMoveEvent,
		TouchEndEvent,
		TouchCancelEvent,
		DisplayExposeEvent,
		DisplayResizeEvent,
		DisplayCloseEvent,
		DisplayLostEvent,
		DisplayFoundEvent,
		DisplaySwitchOutEvent,
		DisplaySwitchInEvent,
		DisplayHaltDrawingEvent,
		DisplayResumeDrawingEvent,
		DisplayConnectedEvent,
		DisplayDisconnectedEvent,
		DisplayOrientationEvent,
		AudioStreamChunkEvent,
		AudioStreamFinishedEvent,
		AudioRecorderChunkEvent,
		NativeDialogCloseEvent,
		MenuClickEvent,
		VideoFrameShowEvent,
		VideoFinishedEvent,
		UserEvent
	>;

	namespace detail {
		struct EventTranslator {
			int typeBegin;
			int typeEnd;
			Event2 (*translator)(const AnyEvent&);
		};

		inline std::vector<EventTranslator> EventTranslators;
	}

	inline Event2 TranslateEvent(const AnyEvent& anyEv) {
		switch(anyEv.type) {
			case ALLEGRO_EVENT_JOYSTICK_AXIS: {
				JoystickAxisEvent ev {};
				ev.joystick = nullptr;
				ev.axis = anyEv.joystick.axis;
				ev.pos = anyEv.joystick.pos;
				ev.stick = anyEv.joystick.stick;
				return ev;
			}
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP: {
				JoystickButtonUpEvent ev {};
				ev.joystick = nullptr;
				ev.button = anyEv.joystick.button;
				return ev;
			}
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN: {
				JoystickButtonDownEvent ev {};
				ev.joystick = nullptr;
				ev.button = anyEv.joystick.button;
				return ev;
			}
			case ALLEGRO_EVENT_KEY_DOWN: {
				KeyDownEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.keycode = static_cast<Key>(anyEv.keyboard.keycode);
			}
			case ALLEGRO_EVENT_KEY_UP: {
				KeyUpEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.keycode = static_cast<Key>(anyEv.keyboard.keycode);
				return ev;
			}
			case ALLEGRO_EVENT_KEY_CHAR: {
				KeyCharEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.keycode = static_cast<Key>(anyEv.keyboard.keycode);
				ev.repeat = anyEv.keyboard.repeat;
				ev.unichar = anyEv.keyboard.unichar;
				ev.modifiers = anyEv.keyboard.modifiers;
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_AXES: {
				MouseMovedEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.delta = {anyEv.mouse.dx, anyEv.mouse.dy};
				ev.pressure = anyEv.mouse.pressure;
				ev.mWheelDelta = anyEv.mouse.dw;
				ev.mWheelPos = anyEv.mouse.w;
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_WARPED: {
				MouseWarpedEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.delta = {anyEv.mouse.dx, anyEv.mouse.dy};
				ev.pressure = anyEv.mouse.pressure;
				ev.mWheelDelta = anyEv.mouse.dw;
				ev.mWheelPos = anyEv.mouse.w;
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				MouseButtonUpEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.mWheelPos = anyEv.mouse.w;
				ev.pressure = anyEv.mouse.pressure;
				ev.button = anyEv.mouse.button;
				if(anyEv.mouse.button == LMB) {
					return LMBUpEvent(ev);
				}
				if(anyEv.mouse.button == RMB) {
					return RMBUpEvent(ev);
				}
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				MouseButtonDownEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.mWheelPos = anyEv.mouse.w;
				ev.pressure = anyEv.mouse.pressure;
				ev.button = anyEv.mouse.button;
				if(anyEv.mouse.button == LMB) {
					return LMBDownEvent(ev);
				}
				if(anyEv.mouse.button == RMB) {
					return RMBDownEvent(ev);
				}
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY: {
				MouseEnterDisplayEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.mWheelPos = anyEv.mouse.w;
				return ev;
			}
			case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY: {
				MouseLeaveDisplayEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.pos = {anyEv.mouse.x, anyEv.mouse.y};
				ev.mWheelPos = anyEv.mouse.w;
				return ev;
			}
			case ALLEGRO_EVENT_TOUCH_BEGIN: {
				TouchBeginEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.id = anyEv.touch.id;
				ev.pos = {anyEv.touch.x, anyEv.touch.y};
				ev.speed = {anyEv.touch.dx, anyEv.touch.dy};
				ev.primary = anyEv.touch.primary;
				return ev;
			}
			case ALLEGRO_EVENT_TOUCH_MOVE: {
				TouchMoveEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.id = anyEv.touch.id;
				ev.pos = {anyEv.touch.x, anyEv.touch.y};
				ev.speed = {anyEv.touch.dx, anyEv.touch.dy};
				ev.primary = anyEv.touch.primary;
				return ev;
			}
			case ALLEGRO_EVENT_TOUCH_END: {
				TouchEndEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.id = anyEv.touch.id;
				ev.pos = {anyEv.touch.x, anyEv.touch.y};
				ev.speed = {anyEv.touch.dx, anyEv.touch.dy};
				ev.primary = anyEv.touch.primary;
				return ev;
			}
			case ALLEGRO_EVENT_TOUCH_CANCEL: {
				TouchCancelEvent ev {};
				ev.sourceDisplay = nullptr;
				ev.id = anyEv.touch.id;
				ev.pos = {anyEv.touch.x, anyEv.touch.y};
				ev.speed = {anyEv.touch.dx, anyEv.touch.dy};
				ev.primary = anyEv.touch.primary;
				return ev;
			}
			case ALLEGRO_EVENT_DISPLAY_EXPOSE: {
				DisplayExposeEvent ev {};
				ev.display = nullptr;
				ev.exposedRect = RectI::XYWH(anyEv.display.x, anyEv.display.y, anyEv.display.width, anyEv.display.height);
				return ev;
			}
			case ALLEGRO_EVENT_DISPLAY_RESIZE: {
				DisplayResizeEvent ev {};
				ev.display = nullptr;
				ev.pos = {anyEv.display.x, anyEv.display.y};
				ev.newSize = {anyEv.display.width, anyEv.display.height};
				return ev;
			}
			case ALLEGRO_EVENT_DISPLAY_CLOSE: {return DisplayCloseEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_LOST: {return DisplayLostEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_FOUND: {return DisplayFoundEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_SWITCH_IN: {return DisplaySwitchInEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT: {return DisplaySwitchOutEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING: {return DisplayHaltDrawingEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING: {return DisplayResumeDrawingEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_CONNECTED: {return DisplayConnectedEvent {DisplayEvent{.display = nullptr}};}
			case ALLEGRO_EVENT_DISPLAY_DISCONNECTED: {return DisplayDisconnectedEvent {DisplayEvent{.display = nullptr}};}



			default: {
				if(anyEv.type < 1024) {
					for(const auto& translator: detail::EventTranslators) {
						if(anyEv.type >= translator.typeBegin && anyEv.type <= translator.typeEnd) {
							return translator.translator(anyEv);
						}
					}
				}

				UserEvent ev {};
				memcpy(ev.data.data(), &anyEv.user.data1, 4 * sizeof(intptr_t));
				return ev;
 			}

		}
	}

	template<typename TVis>
	auto VisitEvent(const AnyEvent& anyEv, TVis&& visitor) {
		auto translatedEvent = TranslateEvent(anyEv);
		return std::visit([&](auto&& event) {
			if constexpr(requires{visitor(event, anyEv.any);}) {
				return visitor(event, anyEv.any);
			} else if constexpr(requires{visitor(event);}) {
				return visitor(event);
			}
		}, translatedEvent);
	}



}


#endif //AXXEGRO_EXPERIMENTALBUILTINEVENTS_HPP
