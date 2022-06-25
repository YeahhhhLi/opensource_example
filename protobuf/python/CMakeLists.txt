# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.20.0)

# 项目信息
# PROJECT(projectname [cxx] [c] [java]) 
PROJECT(MessageTest) # 不指定工程采用的语言，表示 默认支持所有语言

# 安裝需要的包 find_package 默認 先後從 ${CMAKE_MODULE_PATH} 和 ${CMAKE_ROOT} 中查找
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_ROOT = ${CMAKE_ROOT}")
find_package(Protobuf REQUIRED)

# 自定义搜索规则 https://cmake.org/cmake/help/latest/command/file.html
# file(GLOB <variable> [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS] [<globbing-expressions>...])
file(GLOB_RECURSE protobuf_files "*.proto")
message("[debug]  protobuf_files[${protobuf_files}]")

# set(<variable> <value>... [PARENT_SCOPE])
SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
# message("[DEBUG] CMAKE_CURRENT_SOURCE_DIR[${CMAKE_CURRENT_SOURCE_DIR}]")
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

message("[DEBUG] CMAKE_CURRENT_BINARY_DIR[${CMAKE_CURRENT_BINARY_DIR}]")
FOREACH(FIL ${protobuf_files})
  message("[debug]  file[${FIL}]")
  GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
  string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
  string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})
  string(REGEX MATCH "(/proto/.*)" OUT_PATH ${FILE_PATH})
  message("[debug]  OUT_PATH[${OUT_PATH}]")
  set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}_pb2.py")
  EXECUTE_PROCESS(
      COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --python_out=${PROTO_META_BASE_DIR} ${FIL}
  )
  message("Copy ${PROTO_SRCS} To ${FILE_PATH}")
  file(COPY ${PROTO_SRCS} DESTINATION ${FILE_PATH})
  file(COPY ${PROTO_HDRS} DESTINATION ${FILE_PATH})
 
ENDFOREACH()

FILE(GLOB_RECURSE PHDRS "proto/*_pb2.py")

# 指定生成目标
# add_executable(test main.py ${PHDRS} ${PSRCS})

# # TODO 看下这个含义， 缺失这个的时候 总是编译失败: include <proto/meta.pb.h> failed
# target_include_directories(test
#     PUBLIC
#     ${PROTOBUF_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_BINARY_DIR}
# )

# target_link_libraries(test PUBLIC ${Protobuf_LIBRARIES})
 
# set(CMAKE_CXX_STANDARD 11)
