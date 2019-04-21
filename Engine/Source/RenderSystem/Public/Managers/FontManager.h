//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_FONTMANAGER_H
#define BERSERK_FONTMANAGER_H

#include <Strings/String.h>
#include <Foundation/Font.h>
#include <Managers/IFontManager.h>
#include <Managers/ITextureManager.h>
#include <Containers/LinkedList.h>
#include <Importers/IFontImporter.h>

namespace Berserk::Resources
{

    using namespace Importers;

#ifndef PROFILE_FONT_MANAGER
    #define PROFILE_FONT_MANAGER 0
#endif

    /**
     * Default engine font manager
     */
    class ENGINE_API FontManager : public IFontManager
    {
    public:

        /** Want to manually allocate this manager */
        GENARATE_NEW_DELETE(FontManager);

        /** Texture manager to save glyphs maps, and path to the engine fonts folder */
        FontManager(ITextureManager *manager, IFontImporter *importer, IAllocator *allocator,
                    const char *path);

        /** @copydoc IFontManager::~IFontManager() */
        ~FontManager() override;

        /** @copydoc IFontManager::renameFont() */
        void renameFont(IFont* font, const char* name) override;

        /** @copydoc IFontManager::deleteFont() */
        void deleteFont(IFont* font) override;

        /** @copydoc IFontManager::findFont() */
        IFont* findFont(const char* name) override;

        /** @copydoc IFontManager::getFont() */
        IFont* getFont(const char* name) override;

        /** @copydoc IFontManager::loadFont() */
        IFont *loadFont(const char *name, uint32 pixelSize, const char *path) override;

        /** @copydoc IFontManager::loadFontFromXML() */
        IFont* loadFontFromXML(const char* name, XMLNode& node) override;

        /** @copydoc IFontManager::copyFont() */
        IFont* copyFont(IFont* font) override;

        /** @copydoc IFontManager::getDefaultFont() */
        IFont* getDefaultFont() override;

        /** @copydoc IFontManager::getDebugFont() */
        IFont* getDebugFont() override;

        /** @copydoc IFontManager::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IFontManager::getMemoryUsage(sizer) */
        void getMemoryUsage(MemorySizer* sizer) override;

    private:

        static const uint32 INITIAL_FONTS_COUNT = 20;

        /** Store all the managed fonts */
        LinkedList<Font> mFonts;

        /** Path to the font folder */
        CString mFontsPath;

        /** Standard engine font */
        class IFont* mDefaultFont = nullptr;

        /** Mono font for debug purposes */
        class IFont* mDebugFont = nullptr;

        /** Global allocator [pass as param to the fonts] */
        class IAllocator* mAllocator = nullptr;

        /** Store fonts glyphs maps */
        class ITextureManager* mTextureManager = nullptr;

        /** Default importer used to load ttf fonts */
        class IFontImporter* mFontImporter = nullptr;

    };


} // namespace Berserk::Resources

#endif //BERSERK_FONTMANAGER_H