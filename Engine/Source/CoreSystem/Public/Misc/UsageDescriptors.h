//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_USAGEDESCRIPTORS_H
#define BERSERK_USAGEDESCRIPTORS_H

/**
 * Functionality which lays on system memory
 * allocation interfaces or unsafe work with heap
 */
#define MEMORY_API

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

/**
 * Class or functionality which directly interacts with OS
 * IO interface and uses files for loading and saving data
 *
 * Note: this functionality could be slow because of
 * system call and frequent uses of host HDD
 */
#define IO_API

/**
 * Core functionality which does not exported for engine users
 * (Commonly functions used for implementation of the internal
 * engine sub-modules and sub-systems)
 */
#define CORE_API

/**
 * Core functionality for internal engine modules and users
 */
#define CORE_EXPORT

/**
 * Engine functionality which does not exported for engine users
 * (Commonly functions used for implementation of the internal
 * engine sub-modules and sub-systems: resources, manager, drivers)
 */
#define ENGINE_API

/**
 * Common engine functionality for building of application
 * (Virtual classes, interfaces, managers)
 */
#define ENGINE_EXPORT

/**
 * Functionality and classes used only by engine rendering
 * system [data always stored on render thread]
 */
#define RENDER_API

/**
 * Functionality which do not recommended for using by user or
 * engine components
 */
#define ENGINE_DEPRECATED

#endif //BERSERK_USAGEDESCRIPTORS_H
