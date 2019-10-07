cmake_minimum_required (VERSION 3.9)

# list         (APPEND CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
# list         (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Engine/Source/GlfwContext")
# # find_package (GLFW REQUIRED)
# # message      (STATUS "Found GLFW3: headers at ${GLFW_INCLUDE_DIRS}, libraries at ${GLFW_LIBRARYS}")
#
# find_package (OpenGL REQUIRED)
# find_package (glfw3 REQUIRED)
# find_package (GLEW REQUIRED STATIC)
# set          (BERSERK_LINK_THIRD_PARTY ${BERSERK_LINK_THIRD_PARTY} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES})

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(BERSERK_LINK_THIRD_PARTY ${BERSERK_LINK_THIRD_PARTY} ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

if (WIN32)
    # nothing now
endif (WIN32)

set(BERSERK_LINK_THIRD_PARTY
        ${BERSERK_LINK_THIRD_PARTY}
        ${APP_TARGET}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${EXTRA_LIBS}
)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#     -w -framework OpenGL -lGLEW -lGLFW \
#     -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor\
#     -framework COCOA -framework COREVIDEO -framework IOKIT -framework CARBON")

if (APPLE)
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#      -I/usr/local/include -I/opt/X11/include \
#      -L/usr/local/lib -L/opt/X11/lib \
#      -w -framework OpenGL -lGLEW -lGLFW \
#      -lXxf86vm -lpthread -lXrandr -lXinerama -lXi -lXcursor\
#      -framework COCOA -framework COREVIDEO -framework IOKIT -framework CARBON \
#      "
#     )
endif (APPLE)

if (WIN32)

endif (WIN32)

if (UNIX)

endif (UNIX)