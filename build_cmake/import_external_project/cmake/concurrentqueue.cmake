# 添加第三方依赖包
include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif()

set(CONCURRENTQUEUE_GIT_TAG  v1.0.3)  # 指定版本
set(CONCURRENTQUEUE_GIT_URL  git@github.com:cameron314/concurrentqueue.git)  # 指定git仓库地址

FetchContent_Declare(
  concurrentqueue
  GIT_REPOSITORY    ${CONCURRENTQUEUE_GIT_URL}
  GIT_TAG           ${CONCURRENTQUEUE_GIT_TAG}
)

FetchContent_MakeAvailable(concurrentqueue)