//
// Created by Egor Orachyov on 28.07.2018.
//

#include "Objects/Resource.h"

namespace Berserk
{

    const CString& Resource::getResourceFileName() const
    {
        return mResourceName;
    }

} // namespace Berserk