cmake_minimum_required(VERSION 3.10)
project(cannylhismo)

set(OpenCV_DIR C:/OpenCV/opencv/build)
find_package(OpenCV REQUIRED)

add_compile_options(/W4)

add_executable(${PROJECT_NAME} cannylhismo.cpp)

add_subdirectory(cpplot)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} cpplot)