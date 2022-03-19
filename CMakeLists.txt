cmake_minimum_required(VERSION 3.0.0)
project(axxegro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CTest)
enable_testing()

include_directories("deps/include")
include_directories("include")

file(GLOB_RECURSE AXXEGRO_SOURCES "src/*.cpp")

if(WIN32)
	set(CMAKE_CXX_FLAGS "-m32")
endif()

add_library(${PROJECT_NAME} ${AXXEGRO_SOURCES})


option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
	add_subdirectory("examples")
endif()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
