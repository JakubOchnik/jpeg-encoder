include_directories(${INCLUDE_DIR})
target_sources(${PROJECT_NAME} PUBLIC ${SOURCE_DIR}/encoder.cpp)

add_subdirectory(${SOURCE_DIR}/lib)