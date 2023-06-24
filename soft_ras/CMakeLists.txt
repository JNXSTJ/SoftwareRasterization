cmake_minimum_required(VERSION 3.10)
project(Rasterizer)
set(CMAKE_PREFIX_PATH "C:\\Users\\taojian\\Desktop\\opencv\\build_taojian")
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
include_directories(C:\\Users\\taojian\\Desktop\\games101\\作业\\Assignment2\\include)

add_executable(soft_ras MatUtil.cpp MatUtil.hpp soft_ras.cpp world.hpp)
target_link_libraries(soft_ras ${OpenCV_LIBRARIES})

