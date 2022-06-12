#define AXXEGRO_TRUSTED

#include <axxegro/dialog/FileDialog.hpp>

al::FileDialog::FileDialog(
	const std::string& initial_path,
	const std::string& title,
	const std::string& patterns,
	int mode
)	
	: Resource(al_create_native_file_dialog(
		initial_path.c_str(), title.c_str(), patterns.c_str(), mode)
	)
{

}

al::FileDialogResult al::FileDialog::show()
{
	al_show_native_file_dialog(nullptr, ptr());
	int n = al_get_native_file_dialog_count(ptr());
	std::vector<std::string> paths(n);
	for(int i=0; i<n; i++) {
		paths[i] = al_get_native_file_dialog_path(ptr(), i);
	}
	return {paths};
}

std::future<al::FileDialogResult> al::FileDialog::showAsync()
{
	return std::async(&FileDialog::show, this);
}