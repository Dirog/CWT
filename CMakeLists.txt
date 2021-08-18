cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

option(CWT_BUILD_TESTS  "Build tests for cwt"  ON)
option(CWT_BUILD_CUDA   "Build CUDA impl"      ON)
option(CWT_MEASURE_TIME "Do time measurements" ON)

set(PROJECT_NAME cwt)

set(LANGUAGES CXX CUDA)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
if (CWT_BUILD_CUDA)
    set(LANGUAGES ${LANGUAGES} CUDA)
endif()

project(${PROJECT_NAME} LANGUAGES ${LANGUAGES})


include(${CMAKE_CURRENT_SOURCE_DIR}/source/cwt/module.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/source/wavelet/module.cmake)
set(SOURCES ${CWT_SOURCES} ${WAVELET_SOURCES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

if (CWT_BUILD_CUDA)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CUDA_SEPARABLE_COMPILATION  ON
            CUDA_RESOLVE_DEVICE_SYMBOLS OFF)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (CWT_BUILD_TESTS)
    set(TEST_TARGET ${PROJECT_NAME}_test)

    if ((NOT DEFINED CPPUTEST_INCLUDE_DIRS) OR (NOT DEFINED CPPUTEST_LIBRARIES))
        find_package(CppUTest REQUIRED)
    endif( )
    include_directories(${CPPUTEST_INCLUDE_DIRS})

    set(TEST_SOURCES ./source/test/main.cpp
                        ${CWT_TEST_SOURCES})

    add_executable(${TEST_TARGET} ${TEST_SOURCES})

    target_link_libraries(${TEST_TARGET} ${PROJECT_NAME} ${CPPUTEST_LIBRARIES} -lcufft)
endif( )
