project(ColorWheel)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -mssse3 -msse4.1 -msse4.2")
set(ColorWheelLibrarySources colorwheel.cpp form.cpp)
add_library(ColorWheel STATIC ${ColorWheelLibrarySources})
set_property(TARGET ColorWheel PROPERTY CXX_STANDARD 14)
target_link_libraries(ColorWheel Qt5::Core Qt5::Gui Qt5::Widgets)

add_executable(ExampleColorWheel main.cpp)
set_property(TARGET ExampleColorWheel PROPERTY CXX_STANDARD 14)
target_link_libraries(ExampleColorWheel ColorWheel)
