include_directories(${INCLUDE_DIR})
cmake_policy(SET CMP0110 NEW)

add_executable(color_conv ${TEST_DIR}/color_conversion.cpp)
target_link_libraries( color_conv ${OpenCV_LIBS})
add_test("Color conversion (RGB/YCbCr)" color_conv)

#add_executable(hmmm ${TEST_DIR}/abcd.cpp)
#target_link_libraries( hmmm ${OpenCV_LIBS})
#add_test(Eluwina hmmm)