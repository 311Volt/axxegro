//
// Created by volt on 2023-06-26.
//

#ifndef AXXEGRO_EVENTDISPATCHER_HPP
#define AXXEGRO_EVENTDISPATCHER_HPP

#include "../../com/util/Dict.hpp"
#include "UserEvent.hpp"
#include <optional>

namespace al {
	using EventDiscretizerID = uint32_t;

	namespace detail {

		struct EventDiscretizedValue {
			EventDiscretizerID id;
			int64_t value;

			[[nodiscard]] std::size_t hash() const {
				std::size_t ret = 0;
				HashCombine(ret, id);
				HashCombine(ret, value);
				return ret;
			}

			friend auto operator<=>(const EventDiscretizedValue&, const EventDiscretizedValue&) = default;
		};

		struct EventHandlerCoord {
			std::optional<EventType> type;
			std::optional<EventDiscretizedValue> value;

			friend auto operator<=>(const EventHandlerCoord&, const EventHandlerCoord&) = default;
		};

		struct EventHandlerCoordHash {
			std::size_t operator()(const EventHandlerCoord& hc) const {
				std::size_t ret = 1437;
				HashCombine(ret, hc.type);
				if(hc.value.has_value()) {
					HashCombine(ret, hc.value->hash());
				}
				return ret;
			}
		};
	}

	enum EventHandledStatus {
		EventHandled,
		EventNotHandled
	};


	struct IEventHandler {
		virtual EventHandledStatus handle(const Event& ev) = 0;
		virtual ~IEventHandler() = default;
	};

	class GenericEventHandler: public IEventHandler {
	public:
		using HandlerType = std::function<EventHandledStatus(const Event&)>;
		using NoReturnHandlerType = std::function<void(const Event&)>;
		using MinimalHandlerType = std::function<void(void)>;


		template<std::convertible_to<HandlerType> THandler>
		explicit(false) GenericEventHandler(THandler handler) : handler_(std::forward<THandler>(handler))
		{}

		template<std::convertible_to<NoReturnHandlerType> THandler>
			requires (!std::convertible_to<THandler, HandlerType>)
		explicit(false) GenericEventHandler(THandler handler) {
			handler_ = [handler](const Event& ev) {
				handler(ev);
				return EventHandled;
			};
		}

		template<std::convertible_to<MinimalHandlerType> THandler>
		explicit(false) GenericEventHandler(THandler handler) {
			handler_ = [handler]([[maybe_unused]] const Event& ev) {
				handler();
				return EventHandled;
			};
		}

		EventHandledStatus handle(const al::Event &ev) override {
			return handler_(ev);
		}

	protected:
		std::function<EventHandledStatus(const Event&)> handler_;
	};

	template<EventDataType EventT>
	class EventHandler: public IEventHandler {
	public:

		using EventType = EventT;

		using HandlerType = std::function<EventHandledStatus(const EventType&, const AnyEvent&)>;
		using SimplifiedHandlerType = std::function<EventHandledStatus(const EventType&)>;
		using NoReturnHandlerType = std::function<void(const EventType&, const AnyEvent&)>;
		using NoReturnSimplifiedHandlerType = std::function<void(const EventType&)>;
		using MinimalHandlerType = std::function<void(void)>;

		template<std::convertible_to<HandlerType> THandler>
		EventHandler(THandler handler) : handler_(std::move(handler))
		{}

		template<std::convertible_to<SimplifiedHandlerType> THandler>
		EventHandler(THandler handler) {
			handler_ = [handler](const EventType& ev, [[maybe_unused]] const AnyEvent&) {
				return handler(ev);
			};
		}

		template<std::convertible_to<NoReturnHandlerType> THandler>
			requires (!std::convertible_to<THandler, HandlerType>)
		EventHandler(THandler handler) {
			handler_ = [handler](const EventType& ev, const AnyEvent& meta) {
				handler(ev, meta);
				return EventHandled;
			};
		}

		template<std::convertible_to<NoReturnSimplifiedHandlerType> THandler>
			requires (!std::convertible_to<THandler, SimplifiedHandlerType>)
		EventHandler(THandler handler) {
			handler_ = [handler](const EventType& ev, [[maybe_unused]] const AnyEvent&) {
				handler(ev);
				return EventHandled;
			};
		}

		template<std::convertible_to<MinimalHandlerType> THandler>
		EventHandler(THandler handler) {
			handler_ = [handler]([[maybe_unused]] const EventType& ev, [[maybe_unused]] const AnyEvent&) {
				handler();
				return EventHandled;
			};
		}

		EventHandledStatus handle(const Event& ev) override {
			return handler_(EventDataGetter<EventT>{}(ev), ev.any);
		}
	protected:
		std::function<EventHandledStatus(const EventType&, const AnyEvent&)> handler_;
	};

	enum StandardEventDiscretizers {
		ByKeycode = 0,
		ByUnichar = 1,
		ByMouseBtn = 2
	};


	class EventDispatcher {
	public:
		using EventDiscretizer = std::function<int64_t(const Event&)>;

		[[nodiscard("The returned ID of the registered discretizer should be used")]]
		EventDiscretizerID addDiscretizer(EventDiscretizer discretizer) {
			discretizers.emplace_back(std::move(discretizer));
			return discretizers.size() - 1;
		}

		[[nodiscard]] bool isDiscretizerRelevant(EventType eventType, EventDiscretizerID discrId) const {
			if(auto it = relevantDiscretizers.find(eventType); it != relevantDiscretizers.end()) {
				const auto& list = it->second;
				return std::find(list.begin(), list.end(), discrId) != list.end();
			} else {
				return false;
			}
		}



		template<EventDataType EventT>
		EventDispatcher& setEventHandlerForValue(EventType eventType, EventDiscretizerID discrId, int64_t value, EventHandler<EventT> handler) {
			if(!isDiscretizerRelevant(eventType, discrId)) {
				relevantDiscretizers[eventType].push_back(discrId);
			}
			auto key = detail::EventHandlerCoord {
				.type = eventType,
				.value = detail::EventDiscretizedValue {.id = discrId, .value = value}
			};
			handlers[key] = std::make_unique<EventHandler<EventT>>(std::move(handler));
			return *this;
		}

		template<EventDataType EventT>
		EventDispatcher& setEventHandler(EventType eventType, EventHandler<EventT> handler) {
			auto key = detail::EventHandlerCoord {
				.type = eventType,
				.value = std::nullopt
			};
			handlers[key] = std::make_unique<EventHandler<EventT>>(std::move(handler));
			return *this;
		}

		EventDispatcher& setEventHandler(EventType eventType, GenericEventHandler handler) {
			auto key = detail::EventHandlerCoord {
				.type = eventType,
				.value = std::nullopt
			};
			handlers[key] = std::make_unique<GenericEventHandler>(std::move(handler));
			return *this;
		}

		template<UserEventType EventT>
		EventDispatcher& setUserEventHandler(EventHandler<EventT> handler) {
			return setEventHandler<EventT>(UserEventTypeIDGetter<EventT>{}(), std::move(handler));
		}

		EventDispatcher& setCatchallHandler(GenericEventHandler handler) {
			handlers[detail::EventHandlerCoord{}] = std::make_unique<GenericEventHandler>(std::move(handler));
			return *this;
		}

		EventDispatcher& onKeyDown(int keycode, EventHandler<KeyboardEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_KEY_DOWN, ByKeycode, keycode, std::move(handler));
			return *this;
		}

		EventDispatcher& onKeyUp(int keycode, EventHandler<KeyboardEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_KEY_UP, ByKeycode, keycode, std::move(handler));
			return *this;
		}

		EventDispatcher& onKeyChar(int unichar, EventHandler<KeyboardEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_KEY_CHAR, ByUnichar, unichar, std::move(handler));
			return *this;
		}

		EventDispatcher& onKeyCharKeycode(int keycode, EventHandler<KeyboardEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_KEY_CHAR, ByKeycode, keycode, std::move(handler));
			return *this;
		}

		EventDispatcher& onMouseDown(int button, EventHandler<MouseEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, ByMouseBtn, button, std::move(handler));
			return *this;
		}

		EventDispatcher& onMouseUp(int button, EventHandler<MouseEvent> handler) {
			setEventHandlerForValue(ALLEGRO_EVENT_MOUSE_BUTTON_UP, ByMouseBtn, button, std::move(handler));
			return *this;
		}

		EventDispatcher& onMouseMove(EventHandler<MouseEvent> handler) {
			setEventHandler(ALLEGRO_EVENT_MOUSE_AXES, std::move(handler));
			return *this;
		}

		EventHandledStatus dispatch(const Event& event) {
			if(tryDispatchByValue(event) == EventHandled) return EventHandled;
			if(tryDispatchByType(event) == EventHandled) return EventHandled;
			return tryDispatchToCatchall(event);
		}

		EventDispatcher() {
			discretizers.resize(3);
			discretizers[ByKeycode] = [](const Event& ev) {return ev.keyboard.keycode;};
			discretizers[ByUnichar] = [](const Event& ev) {return ev.keyboard.unichar;};
			discretizers[ByMouseBtn] = [](const Event& ev) {return ev.mouse.button;};
		}
	private:

		std::vector<EventDiscretizerID>* findRelevantDiscretizerIDs(EventType type) {
			if(auto itEntry = relevantDiscretizers.find(type); itEntry != relevantDiscretizers.end()) {
				return &itEntry->second;
			}
			return nullptr;
		}

		IEventHandler* findEventHandlerByCoord(const detail::EventHandlerCoord& coord) {
			if(auto itEntry = handlers.find(coord); itEntry != handlers.end()) {
				return itEntry->second.get();
			}
			return nullptr;
		}

		EventHandledStatus tryDispatchCoord(const Event& event, const detail::EventHandlerCoord& coord) {
			if(auto* handler = findEventHandlerByCoord(coord)) {
				return handler->handle(event);
			}
			return EventNotHandled;
		}

		EventHandledStatus tryDispatchByValue(const Event& event) {
			detail::EventHandlerCoord coord = {
				.type = event.type,
				.value = std::nullopt
			};
			auto* ids = findRelevantDiscretizerIDs(event.type);
			if(!ids) {
				return EventNotHandled;
			}
			for(const auto id: *ids) {
				coord.value = detail::EventDiscretizedValue {.id = id, .value = discretizers[id](event)};
				if(tryDispatchCoord(event, coord) == EventHandled) {
					return EventHandled;
				}
			}
			return EventNotHandled;
		}

		EventHandledStatus tryDispatchByType(const Event& event) {
			return tryDispatchCoord(event, {
				.type = event.type,
				.value = std::nullopt
			});
		}

		EventHandledStatus tryDispatchToCatchall(const Event& event) {
			return tryDispatchCoord(event, {});
		}

		std::vector<EventDiscretizer> discretizers;
		std::unordered_map<EventType, std::vector<EventDiscretizerID>> relevantDiscretizers;
		std::unordered_map<detail::EventHandlerCoord, std::unique_ptr<IEventHandler>, detail::EventHandlerCoordHash> handlers;

	};

}

#endif //AXXEGRO_EVENTDISPATCHER_HPP
