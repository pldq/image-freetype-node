project(image_watermark)

set(OPENCV_MAP_IMPORTED_CONFIG "")
set(BUILD_SHARED_LIBS ON)

find_package(OpenCV REQUIRED core imgcodecs imgproc freetype)

if (OPENCV_FOUND)
    message(STATUS "OpenCV:   YES (ver ${OpenCV_VERSION})")
endif ()

add_definitions(-DWATERMARKDLL_EXPORTS)

file(GLOB PUBLIC_HEADERS "../include/*.h")

set(BASE_SRCS
        ${PROJECT_SOURCE_DIR}/main.cpp
)

include_directories(${OPENCV_INCLUDE_DIRS})

add_library(image_watermark
        ${PUBLIC_HEADERS}
        ${BASE_SRCS}
)

if (WIN32)
    if (BUILD_SHARED_LIBS)
        target_compile_definitions(image_watermark PRIVATE DLL_EXPORT)
    endif ()
endif ()

if (BUILD_SHARED_LIBS)
    if (BUILD_FRAMEWORK)
        set_target_properties(image_watermark PROPERTIES
                FRAMEWORK TRUE
                PUBLIC_HEADER "${PUBLIC_HEADERS}"
                PRODUCT_BUNDLE_IDENTIFIER "image_watermark"
                OUTPUT_NAME "image_watermark"
        )
    endif ()
endif ()

target_include_directories(image_watermark
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(image_watermark ${OpenCV_LIBS})

add_custom_command(
        TARGET image_watermark
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${OpenCV_INSTALL_PATH}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/bin ${FREETYPE_PREFIX}/bin ${HARFBUZZ_PREFIX}/bin
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)