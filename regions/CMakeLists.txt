cmake_minimum_required(VERSION 3.10)
project(regions)

set(OpenCV_DIR C:/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

add_executable(${PROJECT_NAME} regions.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})