//
// Created by Egor Orachyov on 31.01.2019.
//

#include "Misc/Assert.h"
#include "Utility/XMLUtility.h"
#include "Strings/StringUtility.h"

namespace Berserk
{

    bool XML::exist(const char *filename)
    {
        FILE* file = fopen(filename, "r");
        auto result = (file != nullptr);
        fclose(file);

        return result;
    }

    bool XML::extension(const char *filename, const char* ext)
    {
        auto pos = Strings<char,'\0'>::strstr(filename, ext);
        return (pos != (-1));
    }

} // namespace Berserk