cmake_minimum_required(VERSION 3.4.1)

project (JobSystem)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(PROJECT_NAME Jobs)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

file(GLOB_RECURSE PROJECT_SOURCE_FILES "source/*.h" "source/*.hpp" "source/*.cpp")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_SOURCE_FILES})


