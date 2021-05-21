add_subdirectory(Core)
add_subdirectory(RHI)

if (BERSERK_WITH_OPENGL)
    message(STATUS "Add Berserk OpenGL RHI backend")
    add_subdirectory(OpenGL)
endif()

if (BERSERK_WITH_VULKAN)
    message(STATUS "Add Berserk Vulkan RHI backend")
    add_subdirectory(Vulkan)
endif()

if (BERSERK_TARGET_LINUX)
    add_subdirectory(Glfw)
    add_subdirectory(Unix)
endif()

if (BERSERK_TARGET_MACOS)
    add_subdirectory(Glfw)
    add_subdirectory(Unix)
endif()
