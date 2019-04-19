//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_FONTMANAGER_H
#define BERSERK_FONTMANAGER_H

#include <Managers/IFontManager.h>
#include <Managers/ITextureManager.h>

namespace Berserk::Resources
{

#ifndef PROFILE_FONT_MANAGER
    #define PROFILE_FONT_MANAGER 1
#endif

    /**
     * Default engine font manager
     */
    class ENGINE_API FontManager : public IFontManager
    {
    public:

        /** Texture manager to save glyphs maps, and path to the engine fonts folder */
        FontManager(ITextureManager* manager, const char* path);

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
        IFont* loadFont(const char* path) override;

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

    };


} // namespace Berserk::Resources

#endif //BERSERK_FONTMANAGER_H