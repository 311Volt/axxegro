function(axxegro_add_copy_resources_command TARGET_NAME SRC_DIR DST_DIR)

    if(${CMAKE_VERSION} VERSION_LESS "3.26.0")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/${SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR}
                )
    else ()
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_SOURCE_DIR}/${SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR}
                )
    endif()

endfunction()

function(axxegro_add_copy_dlls_command TARGET_NAME SRC_DIR DST_DIR)

    file(GLOB AXX_DLL_FILES "${CMAKE_SOURCE_DIR}/dll/*.dll")

    if(WIN32)
        if(${CMAKE_VERSION} VERSION_LESS "3.26.0")
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${AXX_DLL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR}
                    )
        else ()
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${AXX_DLL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${DST_DIR}
                    )
        endif()
    endif ()

endfunction()