//
// Created by Egor Orachyov on 12.07.2018.
//

#ifndef BERSERKENGINE_CONFIGCOMMON_H
#define BERSERKENGINE_CONFIGCOMMON_H

namespace Berserk
{
    /**
     * @defgroup Core
     * @brief Engine core functionality
     *
     * Module provides unified functions: configuration loaders, data containers,
     * memory allocators, common platform flags, asserts, hash functions,
     * log manger and log messages, matrices, vectors, quaternions, math functions,
     * profiling system, wide and ascii static and dynamic strings.
     */

    /**
     * @defgroup Config
     * @brief Application configuration
     *
     * Module provides configuration table, config list and config files loaders
     * to define and setup initial application's values.
     *
     * @ingroup Core
     *
     * @{
     */

    /**
     * Types of cfg loaded params' values
     */
    enum ParamType
    {
        PT_UNDEFINED = 0,
        PT_INT16 = 1,
        PT_INT32 = 2,
        PT_UINT16 = 3,
        PT_UINT32 = 4,
        PT_FLOAT32 = 5,
        PT_FLOAT64 = 6,
        PT_BOOL = 7,
        PT_TEXT = 8
    };

    #define PARAM_TYPE_INT16    "INT16"
    #define PARAM_TYPE_UINT16   "UINT16"
    #define PARAM_TYPE_INT32    "INT16"
    #define PARAM_TYPE_UINT32   "UINT32"
    #define PARAM_TYPE_FLOAT32  "FLOAT32"
    #define PARAM_TYPE_FLOAT64  "FLOAT64"
    #define PARAM_TYPE_BOOL     "BOOL"
    #define PARAM_TYPE_TEXT     "TEXT"

    #define TYPE_NAME           "name"
    #define TYPE_TYPE           "type"
    #define TYPE_DEFAULT        "default"
    #define TYPE_DESCRIPTION    "description"

    /**
     * @}
     */

} // namespace Berserk

#endif //BERSERKENGINE_CONFIGCOMMON_H
