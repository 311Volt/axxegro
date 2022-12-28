#include "axxegro/Init.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

void al::InitJob::execute()
{
	if(!function()) {
		throw InitJobFailed("cannot initialize Allegro: "+name+" failed");
	}
}

void al::Initialize(al::InitJobList list)
{
	al_init();
	for(auto& job: list) {
		job.execute();
	}
}

al::InitJobList al::FullInitJobList()
{
	return {
		AXXEGRO_INIT_JOB(al_install_mouse),
		AXXEGRO_INIT_JOB(al_install_keyboard),
		AXXEGRO_INIT_JOB(al_install_audio),
		AXXEGRO_INIT_JOB(al_init_acodec_addon),
		AXXEGRO_INIT_JOB(al_init_image_addon),
		AXXEGRO_INIT_JOB(al_init_font_addon),
		AXXEGRO_INIT_JOB(al_init_ttf_addon),
		AXXEGRO_INIT_JOB(al_init_native_dialog_addon),
		AXXEGRO_INIT_JOB(al_init_primitives_addon)
	};
}

void al::FullInit()
{
	Initialize(FullInitJobList());
}
