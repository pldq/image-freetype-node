cmake_minimum_required(VERSION 3.14)
project(image_watermark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED core imgcodecs imgproc freetype)

if (OPENCV_FOUND)
    message(STATUS "OpenCV:   YES (ver ${OpenCV_VERSION})")
endif ()

add_definitions(-DWATERMARKDLL_EXPORTS)

if (MSVC)
    add_compile_options("/utf-8")
endif ()

file(GLOB PUBLIC_HEADERS "include/*.h")

set(BASE_SRCS
  ${PROJECT_SOURCE_DIR}/src/main.cpp
)

include_directories(${OPENCV_INCLUDE_DIRS})

add_library(image_watermark
  ${PUBLIC_HEADERS}
  ${BASE_SRCS}
)

target_include_directories(image_watermark
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(image_watermark ${OpenCV_LIBS})