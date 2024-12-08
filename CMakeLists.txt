cmake_minimum_required(VERSION 3.5)

project(vsin01 VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(OpenCV_DIR D:/Forvs/opencv4.10/x64/mingw/lib)
set(OpenCV_DIR D:/msys64/mingw64/lib) #库路径你自己换成你电脑上的

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# 包含 Eigen 头文件目录
include_directories("D:/msys64/mingw64/include/eigen3")

# 查找项目中的所有 .cpp 源文件，排除 CMake 生成目录
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/*.hpp
)

# 手动排除掉 CMake 生成的 CMakeFiles 目录
list(FILTER SOURCES EXCLUDE REGEX "/CMakeFiles/")

# 将找到的文件添加到可执行文件中
add_executable(MYCODE ${SOURCES})

target_link_libraries(MYCODE PRIVATE ${OpenCV_LIBS})
