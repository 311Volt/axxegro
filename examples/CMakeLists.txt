
add_executable(example1 "src/example1.cpp")
target_link_libraries(example1 PUBLIC axxegro)

set(CMAKE_FIND_DEBUG_MODE TRUE)

find_library(ALLEGRO "allegro_monolith-static" "../deps/lib")

target_link_libraries(example1 LINK_PUBLIC ${ALLEGRO})