project(test_image_watermark)

set(TEST_FONTS_PATH ${TEST_FILES_PATH}/fonts)
set(TEST_IMAGES_PATH ${TEST_FILES_PATH}/images)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME} image_watermark)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TEST_FONTS_PATH}/watermark.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_IMAGES_PATH}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_test(NAME generate_watermark
        COMMAND $<TARGET_FILE:${PROJECT_NAME}>
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)




file(GLOB TEST_EXPECTED_FILES ${TEST_IMAGES_PATH}/excepted/*)

foreach(TEST_EXPECTED_FILE ${TEST_EXPECTED_FILES})
    get_filename_component(TEST_NAME ${TEST_EXPECTED_FILE} NAME)
    add_test(
            NAME "check_files_equal ${TEST_NAME}"
            COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_EXPECTED_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME}
    )
endforeach()