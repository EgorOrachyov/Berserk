//
// Created by Egor Orachyov on 17.04.2019.
//

#include <Foundation/Font.h>
#include <Foundation/RenderBase.h>

namespace Berserk::Resources
{

    Font::Font(const char *name, IAllocator *allocator)
            : mCharactersData(INITIAL_CHARS_COUNT, allocator),
              mResourceName(name)
    {

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
            Render::RenderBase::getTextureManager()->deleteTexture(mBitmap);
            mBitmap = nullptr;

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


} // namespace Berserk::Resources