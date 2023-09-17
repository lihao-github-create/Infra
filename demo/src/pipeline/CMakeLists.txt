# 声明当前目录下的源文件
file(GLOB SOURCES "*.cpp")

set(BIN_NAME pipeline)
message("BIN_NAME: ${BIN_NAME}")

# 创建可执行文件
add_executable(${BIN_NAME} ${SOURCES})

# 将 include 目录添加到编译器搜索路径
target_include_directories(${BIN_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接my_class.h所需的其他库或源文件
target_link_libraries(${BIN_NAME} pthread)

# message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

