cmake_minimum_required (VERSION 3.9)

if (APPLE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
     -L/usr/local/Cellar/freeimage/3.17.0_1/lib -lFREEIMAGE \
     "
            )
endif (APPLE)

if (WIN32)

endif (WIN32)

if (UNIX)

endif (UNIX)