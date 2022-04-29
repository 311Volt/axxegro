#ifndef INCLUDE_AXXEGRO_EVENT_USEREVENT
#define INCLUDE_AXXEGRO_EVENT_USEREVENT

#include <allegro5/allegro.h>
#include <memory>
#include <type_traits>

namespace al {
	
	template<typename T, ALLEGRO_EVENT_TYPE _ID> 
	class UserEventType {
	public:
		using DataType = T;
		static constexpr ALLEGRO_EVENT_TYPE ID = _ID;
		static constexpr bool HasOptimization = std::is_trivially_copyable_v<T> && sizeof(T) <= 4*sizeof(intptr_t);
		static constexpr bool CanSkipDtor = std::is_trivially_destructible_v<T>;

		static_assert(ID >= 1024, "Event type IDs less than 1024 are reserved for Allegro5 and addons");

		static ALLEGRO_EVENT Init()
		{
			ALLEGRO_EVENT ev;
			memset(&ev, 0, sizeof(ev));
			ev.any.type = ID;
			return ev;
		}

		static ALLEGRO_EVENT Create(std::unique_ptr<T> dat)
		{
			static_assert(std::is_same_v<typename decltype(dat)::deleter_type, std::default_delete<T>>, "custom deleters are not supported");
			ALLEGRO_EVENT ret = Init();
			
			if constexpr(HasOptimization) {
				memcpy(&ret.user.data1, dat.get(), sizeof(T));
				dat.reset();
			} else {
				ret.user.data1 = (intptr_t)(dat.release());
			}
			return ret;
		}

		static ALLEGRO_EVENT Create(const T& dat)
		{
			static_assert(std::is_copy_constructible_v<T>, "cannot create an event by const-reference from a non-copy constructible type, try the unique_ptr overload");
			ALLEGRO_EVENT ret = Init();
			if constexpr(HasOptimization) {
				memcpy(&ret.user.data1, &dat, sizeof(T));
			} else {
				T* obj = new T(dat);
				ret.user.data1 = (intptr_t)(obj);
			}
			return ret;
		}
	};

	template<typename UEvT>
	void OptUserEventDtor(ALLEGRO_USER_EVENT* ev)
	{
		using DT = typename UEvT::DataType;
		DT* obj = (DT*)(&ev->data1);
		obj->~DT();
	}
	
	template<typename UEvT>
	void DefaultUserEventDtor(ALLEGRO_USER_EVENT* ev)
	{
		using DT = typename UEvT::DataType;
		DT* obj = (DT*)(ev->data1);
		delete obj;
	}

	template<typename UEvT>
	const typename UEvT::DataType& GetUserEventData(const ALLEGRO_EVENT& ev)
	{
#ifndef NDEBUG
		if(ev.type != UEvT::ID) {
			throw std::runtime_error("User event ID mismatch");
		}
#endif
		using DT = typename UEvT::DataType;
		if constexpr(UEvT::HasOptimization) {
			return *(DT*)(&ev.user.data1);
		} else {
			return *(DT*)(ev.user.data1);
		}
	}
}

#endif /* INCLUDE_AXXEGRO_EVENT_USEREVENT */
