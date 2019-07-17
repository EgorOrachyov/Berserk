#ifndef BERSERK_CMAKEOPTIONSLIST_H
#define BERSERK_CMAKEOPTIONSLIST_H

/** Specify Window system */

#define USE_GLFW_SYSTEM

/* Specify your hardware audio driver */

/* #undef USE_OPEN_AL */
/* #undef USE_DIRECT_AUDIO */

/** Font importer flags */

#define USE_FREE_TYPE

/** Image importer flags */

#define USE_FREE_IMAGE

/** Specify engine version via cmake preprocessor */

#define BERSERK_VERSION       "0.3"
#define BERSERK_VERSION_MAJOR 0
#define BERSERK_VERSION_MINOR 3

/* Specify your GPU hardware driver */

/* #undef USE_METAL */
#define USE_OPEN_GL
/* #undef USE_DIRECT_3D */

#endif // BERSERK_CMAKEOPTIONSLIST_H
