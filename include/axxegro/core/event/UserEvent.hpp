#ifndef INCLUDE_AXXEGRO_EVENT_USEREVENT
#define INCLUDE_AXXEGRO_EVENT_USEREVENT

#include "../../common.hpp"
#include "EventSource.hpp"
#include "Event.hpp"

namespace al {

	template<EventType ID>
	constexpr bool IsIDNonReserved = ID >= 1024;


	template<typename T>
	concept UserEventType = requires {
		requires std::copy_constructible<T> || std::move_constructible<T>;
		{ T::EventTypeID } -> std::convertible_to<EventType>;
		requires IsIDNonReserved<T::EventTypeID>;
	};

	template<typename T>
	concept UserEventTypeRef = UserEventType<std::remove_cvref_t<T>>;

	template<UserEventType T>
	constexpr bool CanStoreInDataFields = std::is_trivially_copyable_v<T> && sizeof(T) <= 4*sizeof(intptr_t);


	template<UserEventType EventT>
	inline Event InitUserEvent()
	{
		Event ev;
		memset(&ev, 0, sizeof(ev));
		ev.any.type = EventT::EventTypeID;
		return ev;
	}

	template<UserEventTypeRef EventRefT>
	inline Event CreateUserEvent(EventRefT&& ref)
	{
		using EventT = std::remove_cvref_t<EventRefT>;
		Event ret = InitUserEvent<EventT>();

		if constexpr(CanStoreInDataFields<EventT>) {
			memcpy(&ret.user.data1, &ref, sizeof(EventT));
		} else {
			ret.user.data1 = (intptr_t)(new EventT(std::forward<EventRefT>(ref)));
		}
		return ret;
	}


	template<UserEventType EventT>
	void UserEventDtor(ALLEGRO_USER_EVENT* ev)
	{
		if constexpr (CanStoreInDataFields<EventT>) {
			// trivially copyable implies trivially destructible - nothing to do
		} else {
			auto* obj = (EventT*)(ev->data1);
			delete obj;
		}
	}

	template<UserEventType EventT>
	EventT* GetUserEventDataPtr(const ALLEGRO_USER_EVENT* ev)
	{
		if constexpr (CanStoreInDataFields<EventT>) {
			return (EventT*)(&ev->data1);
		} else {
			return (EventT*)(ev->data1);
		}
	}

	template<UserEventType EventT>
	const EventT& GetUserEventData(const Event& ev)
	{
		if(ev.type != EventT::EventTypeID) {
			throw EventQueueError("User event ID mismatch: expected %d, got %d", EventT::EventTypeID, ev.type);
		}
		return *GetUserEventDataPtr<EventT>(&ev.user);
	}

	template<UserEventType EventT>
	std::optional<std::reference_wrapper<const EventT>> TryGetUserEventData(const Event& ev) {
		if(ev.type != EventT::EventTypeID) {
			return std::nullopt;
		}
		return *GetUserEventDataPtr<EventT>(&ev.user);
	}

	class UserEventSource:
			RequiresInitializables<CoreAllegro>,
			public EventSource
	{
		ALLEGRO_EVENT_SOURCE evs {};
	public:
		UserEventSource(const UserEventSource&) = delete;
		UserEventSource& operator=(const UserEventSource&) = delete;

		UserEventSource() {
			al_init_user_event_source(&evs);
			al_set_event_source_data(&evs, (intptr_t)this);
		}

		~UserEventSource() {
			al_destroy_user_event_source(&evs);
		}

		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return (ALLEGRO_EVENT_SOURCE*)(&evs);
		}

		template<UserEventTypeRef EventRefT>
		void emitEvent(EventRefT&& event) {
			using EventT = std::remove_cvref_t<EventRefT>;
			Event ev = CreateUserEvent(std::forward<EventRefT>(event));
			emitAllegroEvent<EventT>(ev);
		}

	private:
		template<UserEventType EventT>
		void emitAllegroEvent(Event& ev)
		{
			al_emit_user_event(&evs, &ev, UserEventDtor<EventT>);
		}

	};



	inline UserEventSource& GetUserEventSource(const AnyEvent& any) {
		if(any.type < 1024) {
			throw EventSourceError("GetUserEventSource called on non-user event");
		}
		return *(UserEventSource*)al_get_event_source_data(any.source);
	}

	inline UserEventSource& GetUserEventSource(const Event& ev) {
		return GetUserEventSource(ev.any);
	}


	template<typename T>
	concept EventDataType = BuiltinEventType<T> || UserEventType<T>;

	template<BuiltinEventType T>
	inline const T& GetEventData(const Event& ev) {
		return BuiltinEventMember<T>{}(ev);
	}

	template<UserEventType T>
	inline const T& GetEventData(const Event& ev) {
		return GetUserEventData<T>(ev);
	}


}

#endif /* INCLUDE_AXXEGRO_EVENT_USEREVENT */
