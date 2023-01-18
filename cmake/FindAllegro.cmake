find_path(ALLEGRO_INCLUDE_DIR allegro5/allegro.h)
find_library(ALLEGRO_LIBRARY allegro_monolith)

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