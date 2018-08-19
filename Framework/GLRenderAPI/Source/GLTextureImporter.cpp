//
// Created by Egor Orachyov on 19.08.2018.
//

#include "Texture/GLTextureImporter.h"
#include "Texture/GLTextureFormat.h"
#include "Essential/GLDataType.h"
#include "Logging/LogMessages.h"

namespace Berserk
{

    void GLTextureImporter::init()
    {
        mBitmap = nullptr;
        FreeImage_Initialise();
    }

    void GLTextureImporter::destroy()
    {
        mBitmap = nullptr;
        FreeImage_DeInitialise();
    }

    void GLTextureImporter::importTexture(const CHAR *file, UINT32 &width, UINT32 &height, UINT32 &format, UINT32 &type, void *&data)
    {
        unloadData();

        const CHAR* imagePath = file;
        data = nullptr;
        FREE_IMAGE_FORMAT fif;
        fif = FreeImage_GetFileType(imagePath, 0);

        if (fif == FIF_UNKNOWN)
        {
            fif = FreeImage_GetFIFFromFilename(imagePath);
        }

        if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
        {
            mBitmap = FreeImage_Load(fif, imagePath);
        }
        else
        {
            WARNING("GLTextureImporter: Unknown format for image %s", imagePath);
            return;
        }

        if (FreeImage_GetBits(mBitmap) == nullptr) {
            WARNING("GLTextureImporter: Empty bitmap container for image %s", imagePath);
            return;
        }

        if (mBitmap)
        {
            width = FreeImage_GetWidth(mBitmap);
            height = FreeImage_GetHeight(mBitmap);
            format = GLImageFormat::GLIF_BGR;
            type = GLDataType::GLDT_UINT;
            data = (void*)FreeImage_GetBits(mBitmap);
        }
        else
        {
            WARNING("GLTextureImporter: Cannot load bitmap for image %s", imagePath);
            return;
        }
    }

    void GLTextureImporter::importTexture(const CHAR *file, UINT32 &width, UINT32 &height, GLImageFormat &format,
                                          GLDataType &type, void *&data)
    {
        unloadData();

        const CHAR* imagePath = file;
        data = nullptr;
        FREE_IMAGE_FORMAT fif;
        fif = FreeImage_GetFileType(imagePath, 0);

        if (fif == FIF_UNKNOWN)
        {
            fif = FreeImage_GetFIFFromFilename(imagePath);
        }

        if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
        {
            mBitmap = FreeImage_Load(fif, imagePath);
        }
        else
        {
            WARNING("GLTextureImporter: Unknown format for image %s", imagePath);
            return;
        }

        if (FreeImage_GetBits(mBitmap) == nullptr) {
            WARNING("GLTextureImporter: Empty bitmap container for image %s", imagePath);
            return;
        }

        if (mBitmap)
        {
            width = FreeImage_GetWidth(mBitmap);
            height = FreeImage_GetHeight(mBitmap);
            format = GLImageFormat::GLIF_BGR;
            type = GLDataType::GLDT_UBYTE;
            data = (void*)FreeImage_GetBits(mBitmap);
        }
        else
        {
            WARNING("GLTextureImporter: Cannot load bitmap for image %s", imagePath);
            return;
        }
    }

    void GLTextureImporter::unloadData()
    {
        if (mBitmap)
        {
            FreeImage_Unload(mBitmap);
            mBitmap = nullptr;
        }
    }

} // namespace Berserk