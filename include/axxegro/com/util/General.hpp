//
// Created by volt on 2023-07-14.
//

#ifndef AXXEGRO_UTIL_GENERAL_HPP
#define AXXEGRO_UTIL_GENERAL_HPP

#include <span>
#include <cstddef>
#include <algorithm>

namespace al {

	/**
	 * @brief std::span::subspan but it trims the returned span if out of bounds
	 * instead of returning garbage.
	 */
	template<typename T>
	std::span<T> Subspan(std::span<T>& span, ptrdiff_t pos, size_t count) {
		ptrdiff_t begin = std::clamp<ptrdiff_t>(pos, 0, span.size());
		ptrdiff_t end = std::clamp<ptrdiff_t>(pos+count, 0, span.size());
		return span.subspan(begin, end-begin);
	}

	/**
	 * @brief Checks if needle is contained in haystack.
	 */
	template<typename ValT, typename ArrT>
	constexpr bool IsOneOf(ValT&& needle, const ArrT& haystack) {
		for(auto& v: haystack) {
			if(needle == v) {
				return true;
			}
		}
		return false;
	}

}


#endif //AXXEGRO_UTIL_GENERAL_HPP
