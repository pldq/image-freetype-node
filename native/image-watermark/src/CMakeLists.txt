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

add_library(${PROJECT_NAME}
        ${PUBLIC_HEADERS}
        ${BASE_SRCS}
)

if (WIN32)
    if (BUILD_SHARED_LIBS)
        target_compile_definitions(${PROJECT_NAME} PRIVATE DLL_EXPORT)
    endif ()
endif ()

if (BUILD_SHARED_LIBS)
    if (BUILD_FRAMEWORK)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                FRAMEWORK TRUE
                PUBLIC_HEADER "${PUBLIC_HEADERS}"
                PRODUCT_BUNDLE_IDENTIFIER "${PROJECT_NAME}"
                OUTPUT_NAME "${PROJECT_NAME}"
        )
    endif ()
endif ()

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
)

if (WIN32)
    set(DEPENDENCIES_FILES
            ${OpenCV_INSTALL_PATH}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/bin
    )
    message(STATUS "Copy dependencies files ${DEPENDENCIES_FILES} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${DEPENDENCIES_FILES}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif ()

if (CMAKE_JS_VERSION)
    install(IMPORTED_RUNTIME_ARTIFACTS ${OpenCV_LIBS} LIBRARY DESTINATION .)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION .)
else ()
    include(GNUInstallDirs)
    message(STATUS "Build files have been install to: ${CMAKE_INSTALL_PREFIX}")

    install(IMPORTED_RUNTIME_ARTIFACTS ${OpenCV_LIBS} LIBRARY DESTINATION bin)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION bin)
    install(
            # Note the trailing slash in the argument to `DIRECTORY'!
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ # ${OpenCV_INCLUDE_DIRS}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT headers
    )
endif ()