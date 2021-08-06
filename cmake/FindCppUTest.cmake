if (CPPUTEST_LIBRARIES AND CPPUTEST_INCLUDE_DIRS)
    # in cache already
    set(CPPUTEST_FOUND TRUE)
else (CPPUTEST_LIBRARIES AND CPPUTEST_INCLUDE_DIRS)
    find_path(CPPUTEST_INCLUDE_DIR
            NAMES
            CppUTest CppUTestExt
            PATHS
            /usr/include
            /usr/local/include
            /opt/local/include
    )

    find_library(CPPUTEST_LIBRARY
            NAMES
            CppUTest CppUTestExt
            PATHS
            /usr/lib
            /usr/lib/aarch64-linux-gnu
            /usr/local/lib
            /opt/local/lib
            /sw/lib
    )

    set(CPPUTEST_INCLUDE_DIRS
            ${CPPUTEST_INCLUDE_DIR}
    )
    set(CPPUTEST_LIBRARIES
            ${CPPUTEST_LIBRARY}
    )

    if (CPPUTEST_INCLUDE_DIRS AND CPPUTEST_LIBRARIES)
        set(CPPUTEST_FOUND TRUE)
    endif (CPPUTEST_INCLUDE_DIRS AND CPPUTEST_LIBRARIES)

    if (CPPUTEST_FOUND)
        message(STATUS "Found CppUTest:")
        message(STATUS " - Includes: ${CPPUTEST_INCLUDE_DIRS}")
        message(STATUS " - Libraries: ${CPPUTEST_LIBRARIES}")
    else (CPPUTEST_FOUND)
        message(FATAL_ERROR "Could not find CppUTest")
    endif (CPPUTEST_FOUND)

    mark_as_advanced(CPPUTEST_INCLUDE_DIRS CPPUTEST_LIBRARIES)

endif (CPPUTEST_LIBRARIES AND CPPUTEST_INCLUDE_DIRS)