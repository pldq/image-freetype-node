project(test_image_watermark)

add_executable(test_image_watermark main.cpp)

target_include_directories(test_image_watermark
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(test_image_watermark image_watermark)

add_custom_command(TARGET test_image_watermark
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/watermark.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_test(NAME test_image_watermark_generate
        COMMAND $<TARGET_FILE:test_image_watermark>
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
