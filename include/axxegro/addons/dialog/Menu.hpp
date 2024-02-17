
#ifndef AXXEGRO_MENU_HPP
#define AXXEGRO_MENU_HPP

#include <vector>

#include "NativeDialogAddon.hpp"
#include "axxegro/com/Resource.hpp"

namespace al {

	struct MenuItem {
		std::string name;
		std::string displayName;
		bool enabled = true;
	};

	struct CheckboxMenuItem {
		std::string name;
		std::string displayName;
		bool enabled = true;
		bool value = false;
	};

	struct SubmenuMenuItem {
		std::string name;
		std::string displayName;
		bool enabled = true;
	};

	struct MenuSeparator {};


	using MenuEntry = std::variant<MenuItem, CheckboxMenuItem, MenuSeparator>;
	class Menu;

	template<typename T>
	concept MenuEntryType = std::convertible_to<T, MenuEntry> || std::same_as<T, Menu>;

	class Menu:
		Resource<ALLEGRO_MENU>,
		RequiresInitializables<NativeDialogAddon>
	{
	public:

		template<MenuEntryType... Ts>
		explicit Menu(Ts&&... entries) {

		}

	private:

		using InnerMenuOwner = std::unique_ptr<Menu>;
		using Child = std::variant<InnerMenuOwner, MenuEntry>;
		std::vector<Child> children;
	};

}

#endif //AXXEGRO_MENU_HPP
