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
         * Checks whether file has ext extension
         * @param filename File to check
         * @param ext file extension
         * @return True if file has extension ext
         */
        static bool extension(const char* filename, const char* ext);

    };

} // namespace Berserk

#endif //BERSERK_XMLUTILITY_H
