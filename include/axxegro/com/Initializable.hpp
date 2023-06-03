//
// Created by volt on 2023-06-02.
//

#ifndef AXXEGRO_INITIALIZABLE_HPP
#define AXXEGRO_INITIALIZABLE_HPP

#include "Exception.hpp"

#include <type_traits>

#include <allegro5/allegro.h>

namespace al {


	template<typename T>
	concept Initializable = requires {
		{T::name} -> std::convertible_to<const char*>;
		{T::isInitialized()} -> std::same_as<bool>;
		{T::init()} -> std::same_as<bool>;
		T::DependsOn::InitializeAll();
	};

	template<Initializable T, Initializable... TRest>
	inline void Initialize() {
		if(not T::isInitialized()) {
			if(not T::init()) {
				throw InitializeFailed(al::Format("%s could not be initialized.", T::name));
			}
		}
		if constexpr(sizeof...(TRest)) {
			Initialize<TRest...>();
		}
	}

	template<Initializable... TDeps>
	struct InitDependencies {
		static inline void InitializeAll()
		{
			if constexpr(sizeof...(TDeps)) {
				Initialize<TDeps...>();
			}
		}
	};

	template<Initializable T, Initializable... TRest>
	inline void Require()
	{
		if(not T::isInitialized()) [[unlikely]] {
			T::DependsOn::InitializeAll();
			Initialize<T>();
		}
		if constexpr(sizeof...(TRest)) {
			Require<TRest...>();
		}
	}
	
	template<Initializable T, Initializable... TRest>
	inline void InternalRequire()
	{
#ifndef AXXEGRO_NO_AUTO_INIT
		Require<T, TRest...>();
#endif
	}


	template<Initializable TComp1, Initializable... TCompRest>
	struct InitializableRequirement {
		InitializableRequirement() {
			Require<TComp1, TCompRest...>();
		}
	};

	template<Initializable TComp1, Initializable... TCompRest>
	class RequiresInitializables {
#ifndef AXXEGRO_NO_AUTO_INIT
		[[no_unique_address]] InitializableRequirement<TComp1, TCompRest...> axx_comp_req_;
#endif
	};



	struct CoreAllegro {
		static constexpr char name[] = "Allegro";
		[[nodiscard]] static bool isInitialized() {return al_is_system_installed();}
		[[nodiscard]] static bool init() {return al_init();}
		using DependsOn = InitDependencies<>;
	};


}

#endif //AXXEGRO_INITIALIZABLE_HPP
