cmake_minimum_required(VERSION 3.8.0)
project(axxegro VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE AXXEGRO_HEADERS "include/*.hpp")

if (NOT DEFINED AXXEGRO_MASTER_PROJECT)
	set(AXXEGRO_MASTER_PROJECT OFF)
	if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		set(AXXEGRO_MASTER_PROJECT ON)
	endif ()
endif ()

if(NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # workaround for clangd+mingw
# 	set(AXX_TARGET_TRIPLE "i686-w64-mingw32")
# 	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=${AXX_TARGET_TRIPLE}")
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=${AXX_TARGET_TRIPLE}")
# endif()

add_library(${PROJECT_NAME} INTERFACE ${AXXEGRO_HEADERS})

find_package(Allegro REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE ${Allegro_LIBRARIES})
target_include_directories(${PROJECT_NAME} INTERFACE ${Allegro_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} INTERFACE "include")


include("cmake/CopyResources.cmake")

option(AXXEGRO_BUILD_EXAMPLES "Build example programs" ${AXXEGRO_MASTER_PROJECT})
if(AXXEGRO_BUILD_EXAMPLES)
	add_subdirectory("examples")
endif()

option(AXXEGRO_BUILD_DOCS "Build documentation" ${AXXEGRO_MASTER_PROJECT})

find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_OUTPUT_DIRECTORY "docs")
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_ALPHABETICAL_INDEX NO)
	set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_PREDEFINED "ALLEGRO_UNSTABLE")
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
