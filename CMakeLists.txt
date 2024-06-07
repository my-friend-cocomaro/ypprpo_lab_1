cmake_minimum_required(VERSION 3.14)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

set(lib_name "BigArray")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(googletest)

#
file(
    GLOB ${lib_name}_sources
    ${PROJECT_SOURCE_DIR}/include/${lib_name}.h
    ${PROJECT_SOURCE_DIR}/src/${lib_name}.cpp
)

add_library(${lib_name}_lib ${${lib_name}_sources})
#

enable_testing()
add_subdirectory(tests)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${lib_name}_lib)
