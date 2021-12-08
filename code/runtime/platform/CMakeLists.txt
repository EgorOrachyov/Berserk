set(BERSERK_PLATFORM_HEADER
        platform/Application.hpp
        platform/FileSystem.hpp
        platform/Output.hpp
        platform/Window.hpp
        platform/WindowManager.hpp
        )

set(BERSERK_PLATFORM_SRC
        platform/Application.cpp
        platform/FileSystem.cpp
        )

set(BERSERK_PLATFORM_WINDOWS_SRC
        platform/windows/WindowsFileSystem.cpp
        platform/windows/WindowsOutput.cpp
        )

set(BERSERK_PLATFORM_LINUX_SRC
        platform/unix/UnixFileSystem.cpp
        platform/unix/UnixOutput.cpp
        )

set(BERSERK_PLATFORM_MACOS_SRC
        platform/unix/UnixFileSystem.cpp
        platform/unix/UnixOutput.cpp
        )