cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "g++ compiler" FORCE)
set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE STRING "gcc compiler" FORCE)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAG "${CMAKE_C_FLAGS} -g -O0 -lboost_system")


project(KINECT_TEST LANGUAGES C CXX)

set(CMAKE_MODULE_PATH /usr/local/share/cmake-3.14/Modules)

find_package(freenect2 REQUIRED)
find_package(OpenCV REQUIRED)

#set(FREENECT2_INCLUDE_DIRS /home/pemayfes/freenect2/include)
#set(FREENECT2_LIBRARIES /home/pemayfes/freenect2/lib)

file(GLOB MAIN_SOURCES main.cpp src/*.cpp)

include_directories(
    ${freenect2_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    )

add_executable(test ${MAIN_SOURCES})

target_link_libraries(test
    ${freenect2_LIBRARIES}
    ${OpenCV_LIBRARIES})
