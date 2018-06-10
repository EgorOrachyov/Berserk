//
// Created by Egor Orachyov on 30.05.2018.
//

#ifndef BERSERKENGINE_USAGEDESCRIPTORS_H
#define BERSERKENGINE_USAGEDESCRIPTORS_H

// todo: add more details to descriptions

/**
 * Functionality which lays on system memory
 * allocation interfaces or unsafe work with heap
 */
#define MEM_API

/**
 * Functionality which is used to get debug information
 * from systems in run time mode
 */
#define DEBUG_API

/**
 * Functionality used to store engine internal and user data
 * (containers)
 */
#define DATA_API

/**
 * Functionality connected with low level graphical drivers'
 * interfaces
 */
#define GRAPHICS_API

/**
 * Functionality connected with low level audio drivers'
 * interfaces
 */
#define AUDIO_API

#endif //BERSERKENGINE_USAGEDESCRIPTORS_H
