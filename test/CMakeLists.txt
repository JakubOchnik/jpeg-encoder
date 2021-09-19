include_directories(${INCLUDE_DIR})
cmake_policy(SET CMP0110 NEW)

add_subdirectory(${TEST_DIR}/assert)

add_executable(color_conv ${TEST_DIR}/color_conversion.cpp)
target_link_libraries(color_conv PRIVATE smart_assert)
target_include_directories(color_conv PRIVATE ${TEST_DIR}/assert)
add_test("Utils :: Color conversion (RGB<=>YCbCr)" color_conv)

add_executable(mat_get_test ${TEST_DIR}/imgMatrix_get.cpp)
target_link_libraries(mat_get_test INTERFACE matrix mat_get_test PRIVATE smart_assert)
target_include_directories(mat_get_test PRIVATE ${TEST_DIR}/assert)
add_test("ImgMatrix :: Getters" mat_get_test)

add_executable(mat_error_test ${TEST_DIR}/imgMatrix_error.cpp)
target_link_libraries(mat_error_test INTERFACE matrix mat_error_test PRIVATE smart_assert)
target_include_directories(mat_error_test PRIVATE ${TEST_DIR}/assert)
add_test("ImgMatrix :: Error handling" mat_error_test)

add_executable(mat_resize_test ${TEST_DIR}/imgMatrix_resize.cpp)
target_link_libraries(mat_resize_test INTERFACE matrix mat_resize_test PRIVATE smart_assert)
target_include_directories(mat_resize_test PRIVATE ${TEST_DIR}/assert)
add_test("ImgMatrix :: Resize operations" mat_resize_test)

add_executable(sub_mat_get_test ${TEST_DIR}/subMatrix_get.cpp)
target_link_libraries(sub_mat_get_test INTERFACE matrix sub_mat_get_test PRIVATE smart_assert)
target_include_directories(sub_mat_get_test PRIVATE ${TEST_DIR}/assert)
add_test("SubMatrix :: Getters" sub_mat_get_test)

add_executable(dct_test ${TEST_DIR}/dct_methods.cpp)
target_link_libraries(dct_test INTERFACE matrix dct_test PRIVATE smart_assert dct)
target_include_directories(dct_test PRIVATE ${TEST_DIR}/assert)
add_test("DCT :: Various" dct_test)
