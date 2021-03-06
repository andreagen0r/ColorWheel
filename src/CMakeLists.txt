cmake_minimum_required(VERSION 3.9.0)

project(ColorWheel VERSION 1.0.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set( CMAKE_INCLUDE_CURRENT_DIR ON)

set( CMAKE_CXX_STANDARD 14)
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3 -Ofast -march=native")

set( CMAKE_CXX_FLAGS "-msse -msse2 -mssse3 -msse4.1 -msse4.2")


find_package(
    Qt5
    REQUIRED
    COMPONENTS
    Core
    Gui
    Widgets
    )

set( ColorWheelLibrarySources
    colorwheel.cpp
    form.cpp
    )

add_library(
    ColorWheel
    STATIC
    ${ColorWheelLibrarySources}
    )

target_link_libraries(
    ColorWheel
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    )

add_executable( ExampleColorWheel main.cpp)

target_link_libraries( ExampleColorWheel ColorWheel)

