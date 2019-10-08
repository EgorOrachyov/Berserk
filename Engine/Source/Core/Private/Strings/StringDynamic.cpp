//
// Created by Egor Orachyov on 01.07.2019.
//

#include <Strings/StringDynamic.h>

namespace Berserk
{

    /** Reference to the global char string manager */
    StringManager& StringDynamic::manager = StringManager::getCharStringManager();

} // Berserk