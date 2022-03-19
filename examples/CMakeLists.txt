
add_executable(example1 "src/example1.cpp")
target_link_libraries(example1 PUBLIC axxegro)

set(CMAKE_FIND_DEBUG_MODE TRUE)

find_library(FMTLIB "fmt" "../deps/lib")

if(WIN32)
    find_library(ALLEGRO_STATIC "allegro_monolith-static" "../deps/lib")
    target_link_libraries(example1 LINK_PUBLIC ${ALLEGRO_STATIC} ${FMTLIB})
else()
    find_library(ALLEGRO_CORE "allegro")
    find_library(ALLEGRO_MAIN "allegro_main")
    find_library(ALLEGRO_ACODEC "allegro_acodec")
    find_library(ALLEGRO_AUDIO "allegro_audio")
    find_library(ALLEGRO_COLOR "allegro_color")
    find_library(ALLEGRO_DIALOG "allegro_dialog")
    find_library(ALLEGRO_FONT "allegro_font")
    find_library(ALLEGRO_IMAGE "allegro_image")
    find_library(ALLEGRO_MEMFILE "allegro_memfile")
    find_library(ALLEGRO_PHYSFS "allegro_physfs")
    find_library(ALLEGRO_PRIMITIVES "allegro_primitives")
    find_library(ALLEGRO_TTF "allegro_ttf")
    find_library(ALLEGRO_VIDEO "allegro_video")
    
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
    
endif()

add_custom_command(TARGET example1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/examples/resources/ $<TARGET_FILE_DIR:example1>
)