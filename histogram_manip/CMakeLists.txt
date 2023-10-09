cmake_minimum_required(VERSION 3.10)
project(histogram_manip)

set(OpenCV_DIR C:/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} histogram_manip.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})