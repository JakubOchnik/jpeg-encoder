cmake_minimum_required(VERSION 3.0.0)
project(jpeg_enc VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(src)

#target_link_libraries( ${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} INTERFACE img_matrix INTERFACE sub_matrix PUBLIC subsampler)
target_link_libraries( ${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} INTERFACE img_matrix PUBLIC subsampler)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()
add_subdirectory(${TEST_DIR})