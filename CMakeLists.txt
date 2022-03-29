cmake_minimum_required(VERSION 3.8.0)
project(axxegro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

add_library(${PROJECT_NAME} ${AXXEGRO_SOURCES})


option(AXXEGRO_BUILD_EXAMPLES "Build example programs" ON)
if(AXXEGRO_BUILD_EXAMPLES)
	add_subdirectory("examples")
endif()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
