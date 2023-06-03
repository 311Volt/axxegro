//
// Created by volt on 2023-03-05.
//

#ifndef AXXEGRO_TERMINATE_HPP
#define AXXEGRO_TERMINATE_HPP

#include "axxegro/common.hpp"
#include "NativeDialogAddon.hpp"
#include <allegro5/allegro_native_dialog.h>

#include <string>
#include <iostream>

#include "MessageBox.hpp"

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define AXXEGRO_DEMANGLE_EXCEPTION_NAME
#endif

namespace al {

	namespace internal {
		inline std::string DemangleTypeName(const char* name)
		{
#ifdef AXXEGRO_DEMANGLE_EXCEPTION_NAME
			int status;
			char* n = __cxxabiv1::__cxa_demangle(name, nullptr, nullptr, &status);
			return (status==0) ? std::string(n) : std::string(name);
#else
			return name;
#endif
		}
	}


	/**
 * @brief Displays a message box and terminates the program.
 * @details The message box contains the current exception message if found.
 * This function is intended for use as a global terminate handler function.
 */
	inline void Terminate() {
		InternalRequire<NativeDialogAddon>();
		std::string errMsg = "The program exited unexpectedly because of an error.";

		std::string exMsg;
		try {
			if(std::current_exception()) {
				std::rethrow_exception(std::current_exception());
			}
		} catch(const char* msg) {
			exMsg = "Unhandled exception: " + std::string(msg);
		} catch(const std::string& msg) {
			exMsg = "Unhandled exception: " + std::string(msg);
		} catch(std::exception& e) {
			errMsg = "The program exited unexpectedly after throwing an instance of " + internal::DemangleTypeName(typeid(e).name()) + ":";
			exMsg = e.what();
		}

		al::MessageBox("Error", errMsg, exMsg, "", ALLEGRO_MESSAGEBOX_ERROR);

		std::cerr << errMsg << "\n" << exMsg;
		std::abort();
	}

}

#endif //AXXEGRO_TERMINATE_HPP
