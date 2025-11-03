cmake_minimum_required(VERSION 3.15)
project(endoqa LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(ENDOQA_BUILD_TESTS "Build unit tests" ON)

# OpenCV
find_package(OpenCV REQUIRED core imgproc imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

# Third-party single headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Library
add_library(endoqa_lib
    lib/metrics.cpp
    lib/io.cpp
    lib/report.cpp
)
target_include_directories(endoqa_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(endoqa_lib PUBLIC ${OpenCV_LIBS})

# App
add_executable(endoqa app/main.cpp)
target_link_libraries(endoqa PRIVATE endoqa_lib)

# Tests (doctest header-only)
if (ENDOQA_BUILD_TESTS)
    add_executable(endoqa_tests tests/test_metrics.cpp)
    target_link_libraries(endoqa_tests PRIVATE endoqa_lib)
endif()
