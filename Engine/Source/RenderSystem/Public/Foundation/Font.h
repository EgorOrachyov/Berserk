//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_FONT_H
#define BERSERK_FONT_H

#include <Strings/String.h>
#include <Foundation/IFont.h>

namespace Berserk::Resources
{

    /**
     * Default ASCII-7 font
     */
    class ENGINE_API Font : public IFont
    {
    public:

        /** For custom allocation methods */
        GENARATE_NEW_DELETE(GLShader);

        /** Allocator for internal usage */
        Font(const char *name, IAllocator* allocator);

        /** Free internal array */
        ~Font() override = default;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char *getName() override { return mResourceName.get(); }

    public:

        /** @copydoc IFont::() */
        uint32 getPixelWidth() override { return mPixelWidth; }

        /** @copydoc IFont::() */
        uint32 getPixelHeight() override { return mPixelHeight; }

        /** @copydoc IFont::() */
        ITexture* getTexture() override { return mBitmap; }

        /** @copydoc IFont::() */
        ArrayList<Character>* getCharacters() override { return &mCharactersData; }

    public: /*debug*/

        friend class FontManager;

        /** Suppose we work with only ASCII-7 symbols */
        static const uint32 INITIAL_CHARS_COUNT = 128;

        /** Font size in pixel */
        uint32 mPixelWidth = 0;

        /** Font size in pixel */
        uint32 mPixelHeight = 0;

        /** Count of references to this source */
        uint32 mReferenceCount = 0;

        /** Resource name [font name - in engine] */
        CString mResourceName;

        /** All the glyphs data */
        ITexture* mBitmap = nullptr;

        /** Actual per char data */
        ArrayList<Character> mCharactersData;

    };

} // namespace Berserk::Resources

#endif //BERSERK_FONT_H