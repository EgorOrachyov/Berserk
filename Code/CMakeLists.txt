add_subdirectory(BerserkCore)
add_subdirectory(BerserkRHI)

if (BERSERK_WITH_OPENGL)
    message(STATUS "Add Berserk OpenGL RHI backend")
    add_subdirectory(BerserkOpenGL)
endif()

if (BERSERK_TARGET_LINUX)
    add_subdirectory(BerserkGlfw)
    add_subdirectory(BerserkUnix)
endif()

if (BERSERK_TARGET_MACOS)
    add_subdirectory(BerserkGlfw)
    add_subdirectory(BerserkUnix)
endif()