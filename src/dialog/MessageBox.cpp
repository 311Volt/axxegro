#include <axxegro/dialog/MessageBox.hpp>
#include <iostream>
#include <typeinfo>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>

std::string Demangled(const char* name)
{
	int status;
	char* n = __cxxabiv1::__cxa_demangle(name, nullptr, nullptr, &status);
	return (status==0) ? std::string(n) : std::string(name);
}

#else

std::string Demangled(const char* name)
{
	return name;
}

#endif

int al::MessageBox(
	const std::string& title,
	const std::string& heading,
	const std::string& text,
	const std::string& buttons,
	int flags,
	std::optional<std::reference_wrapper<Display>> parentDisplay
)
{
	return al_show_native_message_box(
		parentDisplay ? parentDisplay->get().ptr() : nullptr, 
		title.c_str(), 
		heading.c_str(), 
		text.c_str(), 
		buttons.size() ? buttons.c_str() : nullptr, 
		flags
	);
}

void al::Terminate()
{
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
		errMsg = "The program exited unexpectedly after throwing an instance of " + Demangled(typeid(e).name()) + ":";
		exMsg = e.what();
	}

	al::MessageBox(
		"Error", 
		errMsg, exMsg, 
		"", 
		ALLEGRO_MESSAGEBOX_ERROR
	);

	std::cerr << errMsg << "\n" << exMsg;
	std::abort();
}