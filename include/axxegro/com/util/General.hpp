//
// Created by volt on 2023-07-14.
//

#ifndef AXXEGRO_UTIL_GENERAL_HPP
#define AXXEGRO_UTIL_GENERAL_HPP

#include <span>
#include <cstddef>
#include <algorithm>

namespace al {

	/*
	 * std::span::subspan but it trims the returned span if out of bounds
	 * instead of returning garbage.
	 *
	 * Frustratingly, this was the original behavior of std::span::subspan
	 * during its design phase, before it got nuked instead of being moved
	 * to a renamed version, and in practice I've needed this bound-checked
	 * behavior literally every single time I've ever used this thing.
	 *
	 * committee pls fix
	 */

	template<typename T>
	std::span<T> Subspan(std::span<T>& span, ptrdiff_t pos, size_t count) {
		ptrdiff_t begin = std::clamp<ptrdiff_t>(pos, 0, span.size());
		ptrdiff_t end = std::clamp<ptrdiff_t>(pos+count, 0, span.size());
		return span.subspan(begin, end-begin);
	}

}


#endif //AXXEGRO_UTIL_GENERAL_HPP
