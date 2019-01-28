//
// Created by Egor Orachyov on 19.08.2018.
//

#ifndef BERSERKENGINE_GLTEXTUREIMPORTER_H
#define BERSERKENGINE_GLTEXTUREIMPORTER_H

#include "Render/TextureImporter.h"
#include "FreeImage.h"
#include "Essential/GLDataType.h"
#include "Texture/GLTextureFormat.h"

namespace Berserk
{

    class GLTextureImporter : public TextureImporter
    {
    public:

        virtual ~GLTextureImporter() = default;

        void init() override;
        void destroy() override;

        void importTexture(const CHAR *file, UINT32 &width, UINT32 &height, UINT32 &format, UINT32 &type, void* &data) override;
        void importTexture(const CHAR *file, UINT32 &width, UINT32 &height, GLImageFormat &format, GLDataType &type, void* &data);
        void unloadData() override;

    private:

        FIBITMAP* mBitmap;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTUREIMPORTER_H