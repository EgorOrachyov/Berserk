set(BERSERK_PLATFORM_SRC)
set(BERSERK_PLATFORM_GLFW_SRC)
set(BERSERK_PLATFORM_UNIX_SRC)
set(BERSERK_PLATFORM_WINDOWS_SRC)

if (BERSERK_WITH_GLFW)
    set(BERSERK_PLATFORM_GLFW_SRC
            BerserkPlatform/Glfw/GlfwContext.cpp
            BerserkPlatform/Glfw/GlfwContext.hpp
            BerserkPlatform/Glfw/GlfwWindow.cpp
            BerserkPlatform/Glfw/GlfwWindow.hpp
            BerserkPlatform/Glfw/GlfwWindowManager.cpp
            BerserkPlatform/Glfw/GlfwWindowManager.hpp)
endif ()

if (BERSERK_TARGET_LINUX OR BERSERK_TARGET_MACOS)
    set(BERSERK_PLATFORM_UNIX_SRC
            BerserkPlatform/Unix/UnixSystem.cpp
            BerserkPlatform/Unix/UnixSystem.hpp
            BerserkPlatform/Unix/UnixOut.cpp
            BerserkPlatform/Unix/UnixOut.hpp
            BerserkPlatform/Unix/UnixFileSystem.cpp
            BerserkPlatform/Unix/UnixFileSystem.hpp)
endif()

if (BERSERK_TARGET_WINDOWS)
    set(BERSERK_PLATFORM_WINDOWS_SRC
            BerserkPlatform/Windows/WindowsFileSystem.cpp
            BerserkPlatform/Windows/WindowsFileSystem.hpp
            BerserkPlatform/Windows/WindowsOut.cpp
            BerserkPlatform/Windows/WindowsOut.hpp
            BerserkPlatform/Windows/WindowsSystem.cpp
            BerserkPlatform/Windows/WindowsSystem.hpp)
endif()

set(BERSERK_PLATFORM_HEADER)
set(BERSERK_PLATFORM_SRC
        ${BERSERK_PLATFORM_GLFW_SRC}
        ${BERSERK_PLATFORM_UNIX_SRC}
        ${BERSERK_PLATFORM_WINDOWS_SRC}
        BerserkPlatform/PlatformEntryPoint.cpp
        BerserkPlatform/PlatformFile.cpp
        BerserkPlatform/PlatformFile.hpp
        BerserkPlatform/PlatformOut.hpp
        BerserkPlatform/PlatformStringTable.hpp
        BerserkPlatform/PlatformThread.cpp
        BerserkPlatform/PlatformThread.hpp
        BerserkPlatform/PlatformThreadManager.cpp
        BerserkPlatform/PlatformThreadManager.hpp
        BerserkPlatform/PlatformDialogs.hpp
        BerserkPlatform/PlatformTaskManager.hpp)