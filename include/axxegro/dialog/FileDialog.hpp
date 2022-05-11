#ifndef INCLUDE_AXXEGRO_DIALOG_FILEDIALOG
#define INCLUDE_AXXEGRO_DIALOG_FILEDIALOG

#include <axxegro/resources/Resource.hpp>

#include <allegro5/allegro_native_dialog.h>

#include <vector>
#include <string>
#include <future>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_FILECHOOSER, al_destroy_native_file_dialog);

	struct FileDialogResult {
		const std::vector<std::string> paths;
		inline bool wasCancelled()
		{
			return paths.size() == 0;
		}
	};

	class FileDialog: public Resource<ALLEGRO_FILECHOOSER> {
	public:
		FileDialog(
			const std::string& initial_path = "",
			const std::string& title = "Choose a file...",
			const std::string& patterns = "*.*",
			int mode = 0
		);

		FileDialogResult show();
		std::future<FileDialogResult> showAsync();
	};

}

#endif /* INCLUDE_AXXEGRO_DIALOG_FILEDIALOG */
