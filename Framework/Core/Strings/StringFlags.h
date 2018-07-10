//
// Created by Egor Orachyov on 09.06.2018.
//

#ifndef BERSERKENGINE_STRINGFLAGS_H
#define BERSERKENGINE_STRINGFLAGS_H

namespace Berserk
{

    /**
     * Flags, which shows results of strings' finding methods
     */
    enum StringFindFlags
    {
        NOT_FOUND = -1,
        FOUND = 1
    };

    /**
     * Available types for strings creation
     */
    enum StringType
    {
        ASCII_STATIC_SIZE = 0,
        ASCII_RESIZEABLE = 1,
        UTF32_RESIZEABLE = 2
    };

    /**
     * Desired initial size for string (for static size strings it is const capacity)
     */
    enum StringCapacity
    {
        SC_MAX_CAPACITY_32 = 32,
        SC_MAX_CAPACITY_64 = 64,
        SC_MAX_CAPACITY_128 = 128,
        SC_MAX_CAPACITY_256 = 256,
        SC_MAX_CAPACITY_512 = 512,
        SC_MAX_CAPACITY_1024 = 1024
    };

    /**
     * Count of different dynamically resizeable strings' types in engine
     */
    enum StringBuffersCount
    {
        SBC_CSTRING_BUFFERS_COUNT = 6,
        SBC_WSTRING_BUFFERS_COUNT = 6
    };

} // namespace Berserk

#endif //BERSERKENGINE_STRINGFLAGS_H
