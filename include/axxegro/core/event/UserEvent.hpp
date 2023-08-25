#ifndef INCLUDE_AXXEGRO_EVENT_USEREVENT
#define INCLUDE_AXXEGRO_EVENT_USEREVENT

#include "../../common.hpp"
#include "EventSource.hpp"
#include "EventDataGetter.hpp"
#include "BuiltinEvents.hpp"

#include <atomic>

namespace al {

	namespace detail {
		constexpr EventType AutoEventTypeIDBegin = 0x40000000;
		inline std::atomic<EventType> AutoEventTypeIDCounter = AutoEventTypeIDBegin;

		inline EventType GetNextAutoEventTypeID() {
			if(AutoEventTypeIDCounter == std::numeric_limits<EventType>::max()) {
				throw Exception("Cannot reserve this many event types (how on earth did you trigger this?)");
			}
			return AutoEventTypeIDCounter++;
		}
	}

	template<EventType ID>
	constexpr bool IsIDNonReserved = ID >= 1024;

	template<EventType ID>
	constexpr bool IsIDConstantAllowed = ID < detail::AutoEventTypeIDBegin;

	template<typename T>
	concept UserEventType = requires {
		requires std::copy_constructible<T> || std::move_constructible<T>;
	};

	template<typename T>
	concept UserEventTypeWithConstID = UserEventType<T> && requires {
		{ T::EventTypeID } -> std::convertible_to<EventType>;
		requires IsIDNonReserved<T::EventTypeID>; //low IDs (< 1024) are reserved for addons
		requires IsIDConstantAllowed<T::EventTypeID>; //high IDs (>= 2^30) are reserved for axxegro auto generation
	};

	template<UserEventType EventT>
	struct UserEventTypeIDGetter {
		static inline const EventType EventTypeID = detail::GetNextAutoEventTypeID();
		EventType operator()() const {
			return EventTypeID;
		}
	};

	template<UserEventTypeWithConstID EventT>
	struct UserEventTypeIDGetter<EventT> {
		EventType operator()() const {
			return EventT::EventTypeID;
		}
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
		ev.any.type = UserEventTypeIDGetter<EventT>{}();
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
		auto evTypeID = UserEventTypeIDGetter<EventT>{}();
		if(ev.type != evTypeID) {
			throw EventQueueError("User event ID mismatch: expected %d, got %d", evTypeID, ev.type);
		}
		return *GetUserEventDataPtr<EventT>(&ev.user);
	}

	template<UserEventType EventT>
	std::optional<std::reference_wrapper<const EventT>> TryGetUserEventData(const Event& ev) {
		if(ev.type != UserEventTypeIDGetter<EventT>{}()) {
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
		bool emitEvent(EventRefT&& event) {
			using EventT = std::remove_cvref_t<EventRefT>;
			Event ev = CreateUserEvent(std::forward<EventRefT>(event));
			return emitAllegroEvent<EventT>(ev);
		}

	private:
		template<UserEventType EventT>
		bool emitAllegroEvent(Event& ev)
		{
			void (*dtor)(ALLEGRO_USER_EVENT*) = nullptr;
			if constexpr (not CanStoreInDataFields<EventT>) {
				dtor = UserEventDtor<EventT>;
			}
			return al_emit_user_event(&evs, &ev, dtor);
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

	template<UserEventType T>
	struct EventDataGetter<T> {
		const T& operator()(const Event(ev)) {
			return GetUserEventData<T>(ev);
		}
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_USEREVENT */
