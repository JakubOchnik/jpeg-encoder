include_directories(${INCLUDE_DIR})
cmake_policy(SET CMP0110 NEW)

add_subdirectory(${TEST_DIR}/assert)

add_executable(color_conv ${TEST_DIR}/color_conversion.cpp)
target_link_libraries(color_conv PRIVATE smart_assert)
target_include_directories(color_conv PRIVATE ${TEST_DIR}/assert)
add_test("Utils :: Color conversion (RGB<=>YCbCr)" color_conv)

add_executable(mat_get_test ${TEST_DIR}/matrix_get.cpp)
target_link_libraries(mat_get_test INTERFACE matrix mat_get_test PRIVATE smart_assert)
target_include_directories(mat_get_test PRIVATE ${TEST_DIR}/assert)
add_test("Matrix :: Getters" mat_get_test)

add_executable(mat_error_test ${TEST_DIR}/matrix_error.cpp)
target_link_libraries(mat_error_test INTERFACE matrix mat_error_test PRIVATE smart_assert)
target_include_directories(mat_error_test PRIVATE ${TEST_DIR}/assert)
add_test("Matrix :: Error handling" mat_error_test)