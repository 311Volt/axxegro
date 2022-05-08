

#set(CMAKE_FIND_DEBUG_MODE TRUE)

find_library(ALLEGRO_CORE "allegro" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_MAIN "allegro_main" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_ACODEC "allegro_acodec" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_AUDIO "allegro_audio" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_COLOR "allegro_color" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_DIALOG "allegro_dialog" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_FONT "allegro_font" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_IMAGE "allegro_image" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_MEMFILE "allegro_memfile" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_PHYSFS "allegro_physfs" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_PRIMITIVES "allegro_primitives" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_TTF "allegro_ttf" ${AXXEGRO_DEPS_LIB})
find_library(ALLEGRO_VIDEO "allegro_video" ${AXXEGRO_DEPS_LIB})

set(AXXEGRO_EXAMPLE_LIBS 
    fmt
    axxegro
    ${ALLEGRO_CORE} 
    ${ALLEGRO_MAIN} 
    ${ALLEGRO_ACODEC} 
    ${ALLEGRO_AUDIO} 
    ${ALLEGRO_COLOR} 
    ${ALLEGRO_DIALOG} 
    ${ALLEGRO_FONT} 
    ${ALLEGRO_IMAGE} 
    ${ALLEGRO_MEMFILE} 
    ${ALLEGRO_PHYSFS} 
    ${ALLEGRO_PRIMITIVES} 
    ${ALLEGRO_TTF} 
    ${ALLEGRO_VIDEO}
)

add_custom_target(axxegro_examples ALL)

add_custom_command(TARGET axxegro_examples POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/examples/data/ ${CMAKE_CURRENT_BINARY_DIR}/data
)

if(WIN32)
    add_custom_command(TARGET axxegro_examples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/examples/dll/ ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

function(axxegro_add_example name)
    add_executable("axx_ex_${name}" "src/${name}.cpp")
    add_dependencies("axx_ex_${name}" axxegro_examples)
    target_link_libraries("axx_ex_${name}" LINK_PUBLIC ${AXXEGRO_EXAMPLE_LIBS})
endfunction()


axxegro_add_example("example1")
axxegro_add_example("dvdlogo")
axxegro_add_example("userevents")
axxegro_add_example("vertices")
axxegro_add_example("3dcam")
axxegro_add_example("shader")