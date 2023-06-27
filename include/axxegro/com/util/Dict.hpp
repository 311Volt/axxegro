//
// Created by volt on 2023-06-26.
//

#ifndef AXXEGRO_DICT_HPP
#define AXXEGRO_DICT_HPP

#include <string>
#include <string_view>
#include <unordered_map>

namespace al {

	template <class T>
	inline void HashCombine(std::size_t& seed, const T& v)
	{
		seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

	struct StrHash
	{
		using is_transparent = void;

		size_t operator()(const char* str) const { return std::hash<std::string_view>{}(str); }
		size_t operator()(std::string_view str) const { return std::hash<std::string_view>{}(str); }
		size_t operator()(std::string const& str) const { return std::hash<std::string_view>{}(str); }
	};

	template<typename T>
	using Dict = std::unordered_map<std::string, T, StrHash, std::equal_to<>>;
}

#endif //AXXEGRO_DICT_HPP
