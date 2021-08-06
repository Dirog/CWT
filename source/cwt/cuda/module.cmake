set(CWT_CUDA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/cwt/cuda)

set(CWT_CUDA_SOURCES ${CWT_CUDA_DIR}/cwt.cu)

set(CWT_TEST_CUDA_SOURCES ${CWT_CUDA_DIR}/test.cpp
                          ${CWT_CUDA_DIR}/plot.cpp)