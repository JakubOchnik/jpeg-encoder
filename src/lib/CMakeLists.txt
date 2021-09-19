add_library(subsampler ${SOURCE_DIR}/lib/subsampler.cpp)
add_library(dct ${SOURCE_DIR}/lib/dct.cpp)
target_include_directories(subsampler PUBLIC ${INCLUDE_DIR}/lib)