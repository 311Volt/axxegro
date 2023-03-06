find_path(ALLEGRO_INCLUDE_DIR allegro5/allegro.h)
find_library(ALLEGRO_LIBRARY allegro_monolith)


if(NOT ALLEGRO_LIBRARY)
	find_library(ALLEGRO_CORE allegro)
	find_library(ALLEGRO_MAIN allegro_main)
	find_library(ALLEGRO_ACODEC allegro_acodec)
	find_library(ALLEGRO_AUDIO allegro_audio)
	find_library(ALLEGRO_COLOR allegro_color)
	find_library(ALLEGRO_DIALOG allegro_dialog)
	find_library(ALLEGRO_FONT allegro_font)
	find_library(ALLEGRO_IMAGE allegro_image)
	find_library(ALLEGRO_MEMFILE allegro_memfile)
	find_library(ALLEGRO_PHYSFS allegro_physfs)
	find_library(ALLEGRO_PRIMITIVES allegro_primitives)
	find_library(ALLEGRO_TTF allegro_ttf)
	find_library(ALLEGRO_VIDEO allegro_video)

	set(ALLEGRO_LIBRARY ${ALLEGRO_CORE} ${ALLEGRO_MAIN} ${ALLEGRO_ACODEC} ${ALLEGRO_AUDIO} 
		${ALLEGRO_COLOR} ${ALLEGRO_DIALOG} ${ALLEGRO_FONT} ${ALLEGRO_IMAGE} ${ALLEGRO_MEMFILE} 
		${ALLEGRO_PHYSFS} ${ALLEGRO_PRIMITIVES} ${ALLEGRO_TTF} ${ALLEGRO_VIDEO})
endif()



include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(Allegro
	DEFAULT_MSG
	ALLEGRO_INCLUDE_DIR
	ALLEGRO_LIBRARY
)

mark_as_advanced(ALLEGRO_LIBRARY ALLEGRO_INCLUDE_DIR)

if(Allegro_FOUND)
	set(Allegro_LIBRARIES    ${ALLEGRO_LIBRARY})
	set(Allegro_INCLUDE_DIRS ${ALLEGRO_INCLUDE_DIR})
endif()
