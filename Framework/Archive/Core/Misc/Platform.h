//
// Created by Egor Orachyov on 19.10.2018.
//

#ifndef BERSERK_PLATFORM_H
#define BERSERK_PLATFORM_H

/** Specify your target architecture */

#ifndef TARGET_x86_64
    #define TARGET_x86_64
#endif

/** Specify your target platform */

#ifndef PLATFORM_MAC
    #define PLATFORM_MAC 1
#endif

#ifndef PLATFORM_WINDOWS
    #define PLATFORM_WINDOWS 0
#endif

#ifndef PLATFORM_LINUX
    #define PLATFORM_LINUX 0
#endif

/** Memory settings */

#ifndef VIRTUAL_MEMORY
    #define VIRTUAL_MEMORY
#endif

#ifndef MEMORY_ALIGNMENT
    #define MEMORY_ALIGNMENT 16
#endif

#endif //BERSERK_PLATFORM_H
