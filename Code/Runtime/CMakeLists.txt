add_subdirectory(BerserkCore)
add_subdirectory(BerserkRHI)

if (BERSERK_WITH_OPENGL)
    message(STATUS "Add Berserk OpenGL RHI backend")
    add_subdirectory(BerserkOpenGL)
endif()

if (BERSERK_WITH_VULKAN)
    message(STATUS "Add Berserk Vulkan RHI backend")
    add_subdirectory(BerserkVulkan)
endif()

add_subdirectory(BerserkPlatform)