//
// Created by Egor Orachyov on 31.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/XML/XMLUtility.h"
#include "Public/Strings/StringUtility.h"

namespace Berserk
{

    bool XML::exist(const char *filename)
    {
        FILE* file = fopen(filename, "r");
        auto result = (file != nullptr);
        fclose(file);

        return result;
    }

    bool XML::extension(const char *filename)
    {
        auto pos = Strings<char,'\0'>::strstr(filename, ".xml");
        return (pos != (-1));
    }

} // namespace Berserk