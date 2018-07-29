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
        #define MEMORY_ALIGNMENT 16
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
    #include <cstring>

    /* Buffers common size */

    #define BUFFER_SIZE_8       8
    #define BUFFER_SIZE_16      16
    #define BUFFER_SIZE_32      32
    #define BUFFER_SIZE_64      64
    #define BUFFER_SIZE_128     128
    #define BUFFER_SIZE_256     256
    #define BUFFER_SIZE_512     512
    #define BUFFER_SIZE_1024    1024
    #define BUFFER_SIZE_2048    2048
    #define BUFFER_SIZE_4096    4096

    /* Format for memory consts */

    #define MEM_KiB 1024
    #define MEM_MiB 1048576
    #define MEM_GiB 1073741824

    /* Debug Profiling */
    // todo: create separate debug settings file
    #define DEBUG_NUM_OF_SYSTEMS 4
    #define DEBUG_NO_MEM_LIMITATION 0
    #define DEBUG_CORE 0
    #define DEBUG_DEBUG 1
    #define DEBUG_ALLOCATORS 2
    #define DEBUG_POOLALLOCATORS 3

    #define SAFE_DELETE(pointer) if (pointer) { delete (pointer); (pointer) = NULL; } else {};

#endif // (__x86_64) || (__x86_64__)

#endif //BERSERKENGINE_COMMON_H
