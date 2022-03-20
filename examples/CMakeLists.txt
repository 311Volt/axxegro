
add_executable(example1 "src/example1.cpp")
target_link_libraries(example1 PUBLIC axxegro)

#set(CMAKE_FIND_DEBUG_MODE TRUE)

find_library(FMTLIB "fmt" "../deps/lib")

find_library(ALLEGRO_CORE "allegro" "../deps/lib")
find_library(ALLEGRO_MAIN "allegro_main" "../deps/lib")
find_library(ALLEGRO_ACODEC "allegro_acodec" "../deps/lib")
find_library(ALLEGRO_AUDIO "allegro_audio" "../deps/lib")
find_library(ALLEGRO_COLOR "allegro_color" "../deps/lib")
find_library(ALLEGRO_DIALOG "allegro_dialog" "../deps/lib")
find_library(ALLEGRO_FONT "allegro_font" "../deps/lib")
find_library(ALLEGRO_IMAGE "allegro_image" "../deps/lib")
find_library(ALLEGRO_MEMFILE "allegro_memfile" "../deps/lib")
find_library(ALLEGRO_PHYSFS "allegro_physfs" "../deps/lib")
find_library(ALLEGRO_PRIMITIVES "allegro_primitives" "../deps/lib")
find_library(ALLEGRO_TTF "allegro_ttf" "../deps/lib")
find_library(ALLEGRO_VIDEO "allegro_video" "../deps/lib")

target_link_libraries(example1 LINK_PUBLIC 
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
    ${FMTLIB}
)


add_custom_command(TARGET example1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/examples/data/ $<TARGET_FILE_DIR:example1>/data
)

if(WIN32)
    add_custom_command(TARGET example1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/examples/dll/ $<TARGET_FILE_DIR:example1>
    )
endif()