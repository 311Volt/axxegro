#ifndef INCLUDE_AXXEGRO_DIALOG_FILEDIALOG
#define INCLUDE_AXXEGRO_DIALOG_FILEDIALOG

#include "axxegro/common.hpp"
#include "NativeDialogAddon.hpp"
#include <allegro5/allegro_native_dialog.h>

#include <vector>
#include <string>
#include <future>


namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_FILECHOOSER, al_destroy_native_file_dialog);

	struct FileDialogResult {
		const std::vector<std::string> paths;
		[[nodiscard]] bool wasCancelled() const {
			return paths.empty();
		}
	};

	class FileDialog:
			RequiresInitializables<NativeDialogAddon>,
			public Resource<ALLEGRO_FILECHOOSER> {
	public:
		explicit FileDialog(
			const std::string& initial_path = "",
			const std::string& title = "Choose a file...",
			const std::string& patterns = "*.*",
			int mode = 0
		)
				: Resource(al_create_native_file_dialog(
					initial_path.c_str(), title.c_str(), patterns.c_str(), mode))
		{

		}

		FileDialogResult show() {
			al_show_native_file_dialog(nullptr, ptr());
			int n = al_get_native_file_dialog_count(ptr());
			std::vector<std::string> paths(n);
			for(int i=0; i<n; i++) {
				paths[i] = al_get_native_file_dialog_path(ptr(), i);
			}
			return {paths};
		}

		std::future<FileDialogResult> showAsync() {
			return std::async(&FileDialog::show, this);
		}
	};

}

#endif /* INCLUDE_AXXEGRO_DIALOG_FILEDIALOG */
