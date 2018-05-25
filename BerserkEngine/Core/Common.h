//
// Created by Egor Orachyov on 24.05.2018.
//

#ifndef BERSERKENGINE_COMMON_H
#define BERSERKENGINE_COMMON_H

#if (__x86_64) || (__x86_64__)

    /* Build info */

    #define BUILD_VERSION "0.1"

    /* Define target platform flags */

    #define TARGET_x86_64

    #if (_WIN32) || (__WIN32) || (__WIN32__)
        #define TARGET_PLATFORM_WINDOWS
        #define VIRTUAL_MEMORY
    #endif

    #if (__APPLE__)
        #define TARGET_PLATFORM_MACOS
        #define VIRTUAL_MEMORY
    #endif

    #if (__unix) || (__unix__) || (__linux) || (__linux__)
        #define TARGET_PLATFORM_LINUX
        #define VIRTUAL_MEMORY
    #endif

    /* Include headers */

    #ifdef TARGET_PLATFORM_WINDOWS
        #include <windows.h>
    #endif

    #include <ctime>
    #include <cstdio>
    #include <cwchar>
    #include <cstdlib>

#endif // (__x86_64) || (__x86_64__)

#endif //BERSERKENGINE_COMMON_H
