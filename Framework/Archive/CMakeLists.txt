# Berserk Engine cmake file
# Created by Egor Orachyov

cmake_minimum_required(VERSION 3.9)
project(Berserk)

# Version

set (BERSERK_VERSION_MAJOR 2)
set (BERSERK_VERSION_MINOR 0)

configure_file (
        "${PROJECT_SOURCE_DIR}/Core/Public/Misc/Version.h.in"
        "${PROJECT_BINARY_DIR}/Core/Public/Misc/Version.h"
)

# Compilation properties

set (TARGET Berserk)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

# Platform dependent flags

if (APPLE)
##    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
##     -I/usr/local/include -I/opt/X11/include \
##     -L/usr/local/lib -L/opt/X11/lib \
##     -w -framework OpenGL -lGLEW -lGLFW \
##     -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor\
##     -framework COCOA -framework COREVIDEO -framework IOKIT -framework CARBON \
##     -L/usr/local/Cellar/freeimage/3.17.0_1/lib -I/usr/local/Cellar/freeimage/3.17.0_1/include -lFREEIMAGE \
##     ")
endif (APPLE)
if (WIN32)

endif(WIN32)
if (UNIX)

endif(UNIX)

# Engine Subsystmes' include directories

include_directories(${PROJECT_BINARY_DIR})
include_directories(../../Engine/Source/Core)

# Debug testing files

set (SOURCES
        ../../Engine/Source/Debug/main.cpp
        )

add_executable (${TARGET} ${SOURCES})

# Engine Subsystems

add_subdirectory (../../Engine/Source/Core)

# Link full project

target_link_libraries (Berserk BerserkCore)

# Find OPENGL GLEW GLFW3 PKGCONFIG COCOA IOKIT COREVIDEO

## if (NOT WIN32)
##     set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
##     set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
## endif (NOT WIN32)
##
## find_package (PkgConfig REQUIRED)
## find_package (OpenGL REQUIRED)
##
## if (WIN32)
##     include_directories("C:/GLFW/include" "C:/glew/include")
##     link_directories("C:/glew/lib/Release/x64" "C:/GLFW/lib-mingw" )
## else (WIN32)
##     find_package (glfw3 REQUIRED)
##     find_package (GLEW REQUIRED STATIC)
## endif (WIN32)
## if (APPLE)
##     include_directories(/usr/local/include)
##     find_library(COCOA_LIBRARY Cocoa REQUIRED)
##     find_library(IOKIT_LIBRARY IOKit REQUIRED)
##     find_library(COREVID_LIBRARY CoreVideo REQUIRED)
## endif (APPLE)
##
## # Link libraries to the project
##
## target_link_libraries (${TARGET}
##         ${GLFW3_LIBRARY}
##         ${OPENGL_LIBRARIES}
##         ${GLEW_LIBRARY}
##         ${COCOA_LIBRARY}
##         ${COREVID_LIBRARY}
##         ${IOKIT_LIBRARY})
## if (WIN32)
##     target_link_libraries (${TARGET}
##             ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)
## endif (WIN32)
##
## if (UNIX)
##     target_link_libraries (${TARGET}
##             ${OPENGL_LIBRARIES}
##             ${GLFW3_LIBRARY}
##             ${GLEW_LIBRARY}
##             ${FREEGLUT_LIBRARY}
##             Xxf86vm pthread Xrandr Xinerama Xi Xcursor)
## endif (UNIX)