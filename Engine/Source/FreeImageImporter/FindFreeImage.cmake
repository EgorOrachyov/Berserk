set (FREEIMAGE_FOUND FALSE)
set (FREEIMAGE_INCLUDE_DIRS)
set (FREEIMAGE_LIBRARIES)
set (FREEIMAGE_LIBRARY_DIRS)

set (SEARCH_PATHS
        $ENV{ProgramFiles}/freeimage/include
        $ENV{SystemDrive}/freeimage/include
        $ENV{ProgramFiles}/freeimage
        $ENV{SystemDrive}/freeimage)

if (FREEIMAGE_ROOT)
    set (SEARCH_PATHS
            ${FREEIMAGE_ROOT}
            ${FREEIMAGE_ROOT}/include
            ${SEARCH_PATHS})
endif (FREEIMAGE_ROOT)

FIND_PATH(FREEIMAGE_INCLUDE_DIRS
        NAMES FreeImage.h
        PATHS ${SEARCH_PATHS}
        NO_DEFAULT_PATH)

if (NOT FREEIMAGE_INCLUDE_DIRS)
    FIND_PATH(FREEIMAGE_INCLUDE_DIRS NAMES FreeImage.h)
endif(NOT FREEIMAGE_INCLUDE_DIRS)

if (FREEIMAGE_ROOT)
    set (FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_ROOT})
    if (EXISTS "${FREEIMAGE_ROOT}/lib")
        set (FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_LIBRARY_DIRS} ${FREEIMAGE_ROOT}/lib)
    endif ()
    if (EXISTS "${FREEIMAGE_ROOT}/lib/static")
        set (FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_LIBRARY_DIRS} ${FREEIMAGE_ROOT}/lib/static)
    endif ()
endif (FREEIMAGE_ROOT)

find_library (FREEIMAGE_LIBRARY
        NAMES freeimage
        PATHS ${FREEIMAGE_LIBRARY_DIRS}
        NO_DEFAULT_PATH)

if (NOT FREEIMAGE_LIBRARY)
    find_library (FREEIMAGE_LIBRARY NAMES freeimage)
endif(NOT FREEIMAGE_LIBRARY)

if (FREEIMAGE_LIBRARY)
    set (FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
endif (FREEIMAGE_LIBRARY)

if (FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
    set (FREEIMAGE_FOUND TRUE)
    if (NOT FREEIMAGE_FIND_QUIETLY)
        message (STATUS "Found FreeImage: headers at ${FREEIMAGE_INCLUDE_DIRS}, libraries at ${FREEIMAGE_LIBRARIES}")
    endif(NOT FREEIMAGE_FIND_QUIETLY)
else (FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
    set (FREEIMAGE_FOUND FALSE)
    if (FREEIMAGE_FIND_REQUIRED)
        message (STATUS "FreeImage not found")
    endif(FREEIMAGE_FIND_REQUIRED)
endif(FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)

mark_as_advanced(FREEIMAGE_INCLUDE_DIRS FREEIMAGE_LIBRARIES)