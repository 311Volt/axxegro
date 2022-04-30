cmake_minimum_required(VERSION 3.8.0)
project(axxegro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(CTest)
enable_testing()

include_directories("deps/include")
include_directories("include")


file(GLOB_RECURSE AXXEGRO_SOURCES "src/*.cpp")

# set(CMAKE_VERBOSE_MAKEFILE ON)

get_filename_component(AXXEGRO_DEPS_LIB "deps/lib" REALPATH)

if(WIN32)
	set(CMAKE_C_FLAGS "-m32")
	set(CMAKE_CXX_FLAGS "-m32 -Wall -Wextra -pedantic")
endif()

add_subdirectory("fmt")
include_directories("fmt/include")

add_library(${PROJECT_NAME} ${AXXEGRO_SOURCES})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC fmt)

option(AXXEGRO_BUILD_EXAMPLES "Build example programs" ON)
if(AXXEGRO_BUILD_EXAMPLES)
	add_subdirectory("examples")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)