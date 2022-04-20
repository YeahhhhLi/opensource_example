# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.20.0)

# 项目信息
# PROJECT(projectname [cxx] [c] [java]) 
PROJECT(Test) # 不指定工程采用的语言，表示 默认支持所有语言

# 安裝需要的包 find_package 默認 先後從 ${CMAKE_MODULE_PATH} 和 ${CMAKE_ROOT} 中查找
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_ROOT = ${CMAKE_ROOT}")

find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)
find_package(Protobuf REQUIRED)

# if (Protobuf_FOUND)
#     #打印相关的信息
#     message("Protobuf_FOUND:${Protobuf_FOUND}")
#     message("Protobuf_version:${Protobuf_VERSION}")
#     message("Protobuf_include:${Protobuf_INCLUDE_DIR}")
#     message("protobuf_library:${Protobuf_LIBRARY}")
# endif()

# 自定义搜索规则 https://cmake.org/cmake/help/latest/command/file.html
# file(GLOB <variable> [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS] [<globbing-expressions>...])
file(GLOB protobuf_files "proto/*.proto")

# set(<variable> <value>... [PARENT_SCOPE])
SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
# message("[DEBUG] CMAKE_CURRENT_SOURCE_DIR[${CMAKE_CURRENT_SOURCE_DIR}]")
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(FIL ${protobuf_files})
  GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
  string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
  string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})
  string(REGEX MATCH "(/proto.*)" OUT_PATH ${FILE_PATH})
  set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.cc")
  set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.h")
  EXECUTE_PROCESS(
      COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
  )
  file(COPY ${PROTO_SRCS} DESTINATION ${FILE_PATH})
  file(COPY ${PROTO_HDRS} DESTINATION ${FILE_PATH})
 
ENDFOREACH()

FILE(GLOB PHDRS "proto/*.pb.h")
FILE(GLOB PSRCS "proto/*.pb.cc")

# 指定生成目标
add_executable(test main.cc ${PHDRS} ${PSRCS})

# TODO 看下这个含义， 缺失这个的时候 总是编译失败: include <proto/meta.pb.h> failed
target_include_directories(test
    PUBLIC
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(test PUBLIC arrow_shared parquet_shared ${Protobuf_LIBRARIES})
 
set(CMAKE_CXX_STANDARD 11)
