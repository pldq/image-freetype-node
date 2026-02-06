project(image_watermark)

set(OPENCV_MAP_IMPORTED_CONFIG "")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(image_watermark
        ${OpenCV_LIBS}
)

if (WIN32)
    set(DEPENDENCIES_FILES
            ${OpenCV_INSTALL_PATH}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/bin
    )
    message(STATUS "Copy dependencies files ${DEPENDENCIES_FILES} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    add_custom_command(
            TARGET image_watermark
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${DEPENDENCIES_FILES}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif ()

include(GNUInstallDirs)

install(IMPORTED_RUNTIME_ARTIFACTS ${OpenCV_LIBS} LIBRARY DESTINATION bin)

install(TARGETS image_watermark LIBRARY DESTINATION bin)

install(
        # Note the trailing slash in the argument to `DIRECTORY'!
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ # ${OpenCV_INCLUDE_DIRS}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT headers
)

message(STATUS "Build files have been install to: ${CMAKE_INSTALL_PREFIX}")
