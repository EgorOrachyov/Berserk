//
// Created by Egor Orachyov on 28.03.2019.
//

#include "Helpers/MaterialManagerHelper.h"

namespace Berserk::Resources
{

    bool MaterialManagerHelper::import(IMaterial *material, XMLNode &node, const CString &path)
    {

    }

    uint32 MaterialManagerHelper::getMaterialType(const char *source)
    {
        uint32 result = 0;
        uint32 i = 0;

        while (source[i] != '\0')
        {
            result = result << 1;

            if (source[i] == '1')
            {
                result += 1;
            }

            i += 1;
        }

        return result;
    }

    Vec4f MaterialManagerHelper::getColorRGBA(const char *r, const char *g, const char *b, const char *a)
    {
        Vec4f result;

        if (r) result.x = (float32) atof(r);
        if (g) result.y = (float32) atof(g);
        if (b) result.z = (float32) atof(b);
        if (a) result.w = (float32) atof(a);

        return result;
    }

} // namespace Berserk::Resources