cmake_minimum_required(VERSION 3.0.0)
project(my_project)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置C标准
set(CMAKE_C_STANDARD 17)

# 设置默认的构建类型为Release，如果用户未指定，则使用Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


# 设置交叉编译工具链（根据您的交叉编译工具链的路径和名称进行修改）
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# 设置全局的编译标志，将警告视为错误
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# 设置Debug和Release构建类型的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 构建类型的编译选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
else()
    # Release 构建类型的编译选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG -Wall")
    # 设置目标架构为x86_64
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64")
    # # 设置目标架构为aarch64
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")

endif()

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out)
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# 添加子目录
add_subdirectory(src)
add_subdirectory(demo)




