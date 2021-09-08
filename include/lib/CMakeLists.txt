add_library(base_matrix INTERFACE ${INCLUDE_DIR}/lib/baseMatrix.hpp)
target_include_directories(base_matrix INTERFACE . PUBLIC ${INCLUDE_DIR}/lib)

add_library(img_matrix INTERFACE ${INCLUDE_DIR}/lib/imgMatrix.hpp.hpp)
target_include_directories(img_matrix INTERFACE .)