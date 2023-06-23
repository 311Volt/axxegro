//
// Created by volt on 2023-06-18.
//

#ifndef AXXEGRO_METAPROGRAMMING_HPP
#define AXXEGRO_METAPROGRAMMING_HPP

#include <type_traits>
#include <concepts>

namespace al {
	template<typename T>
	concept Number = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

	template<typename T>
	concept Integer = std::is_integral_v<T> && !std::is_same_v<T, bool>;

#define AXXEGRO_STATIC_ASSERT_FALSE(T, msg) static_assert(requires(T x) { x.b8c46e1725d14d6b84ca; }, msg)
	
}

#endif //AXXEGRO_METAPROGRAMMING_HPP
