//
// Created by Egor Orachyov on 17.04.2019.
//

#include <Foundation/Font.h>
#include <Foundation/RenderBase.h>

namespace Berserk::Resources
{

    Font::Font(const char *name, IAllocator *allocator, ITextureManager *manager)
            : mCharactersData(INITIAL_CHARS_COUNT, allocator),
              mResourceName(name),
              mTextureManager(manager)
    {
        FAIL(manager, "Null pointer texture manager");

        mBitmap = mTextureManager->createTexture(name);
        mTextureManager->bindSampler(mBitmap, mTextureManager->getSamplerLinear());
    }

    void Font::addReference()
    {
        mReferenceCount += 1;
    }

    void Font::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0 && mBitmap != nullptr)
        {
            mTextureManager->deleteTexture(mBitmap);
            mBitmap = nullptr;
            mTextureManager = nullptr;

            delete (&mResourceName);
            delete (&mCharactersData);
        }
    }

    uint32 Font::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 Font::getMemoryUsage()
    {
        return sizeof(Font) + mCharactersData.getMemoryUsage();
    }

    void Font::set(uint32 pixelWidth, uint32 pixelHeight)
    {
        mPixelWidth = pixelWidth;
        mPixelHeight = pixelHeight;
    }


} // namespace Berserk::Resources