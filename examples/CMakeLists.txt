
add_custom_target(axxegro_examples ALL)

add_custom_command(TARGET axxegro_examples POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/examples/data/ ${CMAKE_CURRENT_BINARY_DIR}/data
)

# if(WIN32)
#     add_custom_command(TARGET axxegro_examples POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/examples/dll/ ${CMAKE_CURRENT_BINARY_DIR}
#     )
# endif()

function(axxegro_add_example name)
    add_executable("axx_ex_${name}" "src/${name}.cpp")
    add_dependencies("axx_ex_${name}" axxegro_examples)
    target_link_libraries("axx_ex_${name}" axxegro)
endfunction()


axxegro_add_example("example1")
axxegro_add_example("dvdlogo")
axxegro_add_example("userevents")
axxegro_add_example("vertices")
axxegro_add_example("3dcam")
axxegro_add_example("shader")
axxegro_add_example("fpsmap")
axxegro_add_example("audio")
axxegro_add_example("video")
axxegro_add_example("subbitmap")
axxegro_add_example("benchmark")
