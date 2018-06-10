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
    enum StringInitialSize
    {
        MAX_CAPACITY_32 = 0,
        MAX_CAPACITY_64 = 1,
        MAX_CAPACITY_128 = 2,
        MAX_CAPACITY_256 = 3,
        MAX_CAPACITY_512 = 4,
        MAX_CAPACITY_1024 = 5
    };

} // namespace Berserk

#endif //BERSERKENGINE_STRINGFLAGS_H
