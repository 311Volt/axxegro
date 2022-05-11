cmake_minimum_required(VERSION 3.8.0)
project(axxegro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(CTest)
enable_testing()



file(GLOB_RECURSE AXXEGRO_SOURCES "src/*.cpp")

# set(CMAKE_VERBOSE_MAKEFILE ON)

get_filename_component(AXXEGRO_DEPS_LIB "deps/lib" ABSOLUTE)

set(AXXEGRO_WIN64 OFF)
if(WIN32)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
	if(NOT AXXEGRO_WIN64)
		set(CMAKE_C_FLAGS "-m32")
		set(CMAKE_CXX_FLAGS "-m32")
	endif()
endif()


add_library(${PROJECT_NAME} ${AXXEGRO_SOURCES})
# target_link_libraries(${PROJECT_NAME} LINK_PUBLIC fmt)

target_include_directories(${PROJECT_NAME} PUBLIC "deps/include")
target_include_directories(${PROJECT_NAME} PUBLIC "include")

option(AXXEGRO_BUILD_EXAMPLES "Build example programs" ON)
if(AXXEGRO_BUILD_EXAMPLES)
	add_subdirectory("examples")
endif()

option(AXXEGRO_BUILD_DOCS "Build documentation" ON)
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_OUTPUT_DIRECTORY "docs")
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

	if(AXXEGRO_BUILD_DOCS)
		doxygen_add_docs(
			axxegro_docs
			${CMAKE_CURRENT_SOURCE_DIR}/"include/axxegro"
			ALL
		)
	endif()
	
else(DOXYGEN_FOUND)
	message("Doxygen not found, won't generate docs")
endif(DOXYGEN_FOUND)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)