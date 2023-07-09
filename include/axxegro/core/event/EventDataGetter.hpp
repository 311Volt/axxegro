//
// Created by volt on 2023-07-09.
//

#ifndef AXXEGRO_EVENTDATAGETTER_HPP
#define AXXEGRO_EVENTDATAGETTER_HPP

#include "../../com/util/Metaprogramming.hpp"

namespace al {

	using Event = ALLEGRO_EVENT;

	template<typename T>
	struct EventDataGetter {
		AXXEGRO_STATIC_ASSERT_FALSE(T, "This type cannot be interpreted as an Allegro event");
	};

	template<typename T>
	concept EventDataType = requires(const Event& ev) {
		{EventDataGetter<T>{}(ev)} -> std::convertible_to<const T&>;
	};

}


#endif //AXXEGRO_EVENTDATAGETTER_HPP
