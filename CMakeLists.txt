cmake_minimum_required(VERSION 3.21)

project(image-freetype-node)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(TEST_FILES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/testfiles)

if (MSVC)
    add_compile_options("/utf-8")
endif ()

enable_testing()
if (CMAKE_JS_VERSION)
    set(NODE_ADDON_INSTALL_BIN_DIR "${NODE_ADDON_INSTALL_PREFIX}/${NODE_ADDON_PLATFORM}/${NODE_ADDON_ARCH}")
    cmake_path(NORMAL_PATH NODE_ADDON_INSTALL_BIN_DIR)
    message(STATUS "NODE_ADDON_INSTALL_BIN_DIR: ${NODE_ADDON_INSTALL_BIN_DIR}")

    set(CMAKE_INSTALL_PREFIX ${NODE_ADDON_INSTALL_BIN_DIR})
endif ()

add_subdirectory(native/image-watermark)

if (CMAKE_JS_VERSION)
    add_subdirectory(native/node-watermark-loader)
endif ()