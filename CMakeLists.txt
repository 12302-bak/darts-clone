cmake_minimum_required(VERSION 3.10)

project(darts-clone)
set(CMAKE_CXX_STANDARD 17)

set(JAVA_HOME "/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home")
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH "/Users/stevenobelia/Documents/project_idea_test/darts-clone-java/src/main/resources/jp/darts/native/Mac/x86_64")

# 添加头文件路径（包括 JNI）
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${JAVA_HOME}/include
    ${JAVA_HOME}/include/darwin   # 根据平台选择 win32、linux 或 darwin
)

file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
list(REMOVE_ITEM SRC_LIST
        "${CMAKE_CURRENT_SOURCE_DIR}/src/darts.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/darts-benchmark.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mkdarts.cc"
)

#  two cmake application，one of library ,and other one is executable
add_library(darts-java SHARED ${SRC_LIST})
add_executable(darts src/darts.cc)

# target_link_libraries("-framework CoreFoundation" "-framework SystemConfiguration")





