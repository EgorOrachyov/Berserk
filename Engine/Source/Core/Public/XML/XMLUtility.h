//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_XMLUTILITY_H
#define BERSERK_XMLUTILITY_H

namespace Berserk
{

    class XML
    {
    public:

        /**
         * Checks whether the file with that name exist
         * @param filename Config file
         * @return True if file exist
         */
        static bool exist(const char* filename);

        /**
         * Checks whether file has .xml extension
         * @param filename File to check
         * @return True if file has .xml extension
         */
        static bool extension(const char* filename);

    };

} // namespace Berserk

#endif //BERSERK_XMLUTILITY_H
