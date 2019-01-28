//
// Created by Egor Orachyov on 19.08.2018.
//

#ifndef BERSERKENGINE_TEXTUREIMPORTER_H
#define BERSERKENGINE_TEXTUREIMPORTER_H

#include "Misc/Types.h"

namespace Berserk
{

    class TextureImporter
    {
    public:

        virtual ~TextureImporter() = default;

        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void importTexture(const CHAR *file, UINT32 &width, UINT32 &height, UINT32 &format, UINT32 &type, void *&data) = 0;
        virtual void unloadData() = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_TEXTUREIMPORTER_H
