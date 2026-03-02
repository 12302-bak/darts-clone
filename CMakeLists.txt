cmake_minimum_required(VERSION 3.10)

project(darts-clone)
set(CMAKE_CXX_STANDARD 17)

# Release
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

message(STATUS "JAVA_HOME = $ENV{JAVA_HOME}")
# actions 中每个平台都有 JAVA_HOME 环境变量，所以不会进入 if,仅记录。
if(NOT DEFINED ENV{JAVA_HOME})
    find_package(JNI REQUIRED)
    # https://cmake.org/cmake/help/latest/module/FindJNI.html
    include_directories(${PROJECT_SOURCE_DIR}/include ${JNI_INCLUDE_DIRS})
else()
    set(JAVA_HOME $ENV{JAVA_HOME})
    include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${JAVA_HOME}/include
        ${JAVA_HOME}/include/darwin
        ${JAVA_HOME}/include/linux
        ${JAVA_HOME}/include/win32
    )
endif()

file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
list(REMOVE_ITEM SRC_LIST
        "${CMAKE_CURRENT_SOURCE_DIR}/src/darts.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/darts-benchmark.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mkdarts.cc"
)

#  two cmake application，one of library ,and other one is executable
add_library(darts-java SHARED ${SRC_LIST})

# 设置输出名称为 libdarts-java.dylib（macOS 默认加 lib 前缀）
# set_target_properties(darts-java PROPERTIES
#    PREFIX "lib"
#    OUTPUT_NAME "darts-java"
#)

# 可执行文件（可选）
# add_executable(darts src/darts.cc)

# target_link_libraries("-framework CoreFoundation" "-framework SystemConfiguration")





