//
// Created by Egor Orachyov on 17.04.2019.
//

#include "Managers/FontManager.h"

namespace Berserk::Resources
{

    FontManager::FontManager(ITextureManager *manager, IFontImporter *importer, IAllocator *allocator,
                                 const char *path)
            : mFonts(INITIAL_FONTS_COUNT),
              mTextureManager(manager),
              mFontImporter(importer),
              mAllocator(allocator),
              mFontsPath(path)
    {
        FAIL(manager, "Null pointer texture manager");
        FAIL(importer, "Null pointer font importer");
        FAIL(path, "Null pointer font folder path");

        mDefaultFont = loadFont("DefaultFont", 48, "{FONTS}/Arial.ttf");
        mDebugFont = loadFont("DebugFont", 48, "{FONTS}/Vera.ttf");

        PUSH("FontManager: initialize");
    }

    FontManager::~FontManager()
    {
        {
            // Explicit deletions for fonts in the manager,
            // Which references were not decreased to the 0
            // (Avoids memory leaks)

            for (auto current = mFonts.iterate(); current != nullptr; current = mFonts.next())
            {
#if PROFILE_FONT_MANAGER
                PUSH("FontManager: release font [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        PUSH("FontManager: de-initialize");
    }

    void FontManager::renameFont(IFont *font, const char *name)
    {
#if PROFILE_FONT_MANAGER
        PUSH("FontManager: rename font [old name: '%s'][new name: '%s']", font->getName(), name);
#endif

        auto renamed = dynamic_cast<Font*>(font);
        renamed->mResourceName = name;
    }

    void FontManager::deleteFont(IFont *font)
    {
        CText name(font->getName());
        font->release();

        if (font->getReferenceCount() == 0)
        {
#if PROFILE_FONT_MANAGER
            PUSH("FontManager: delete font [name: '%s']", name.get());
#endif
            mFonts.remove((Font*)font);
        }
    }

    IFont* FontManager::findFont(const char *name)
    {
        for (auto current = mFonts.iterate(); current != nullptr; current = mFonts.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IFont* FontManager::getFont(const char *name)
    {
        IFont* found = findFont(name);

        if (found)
        {
            found->addReference();
#if PROFILE_FONT_MANAGER
            PUSH("FontManager: find font [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        return nullptr;
    }

    IFont* FontManager::loadFont(const char *name, uint32 pixelSize, const char *path)
    {
        IFont* found = findFont(name);

        if (found)
        {
            found->addReference();
#if PROFILE_FONT_MANAGER
            PUSH("FontManager: find font [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
            return found;
        }

        CPath filename(path);
        filename = filename.replace(CPath("{FONTS}"), CPath(mFontsPath.get()));

        auto font = new(mFonts.preallocate()) Font(name, mAllocator, mTextureManager);
        auto success = mFontImporter->import(filename.get(), pixelSize, font);

        if (!success)
        {
            WARNING("Cannot load font from file [name: '%s']", path);

            font->release();
            mFonts.remove(font);
            return getDefaultFont();
        }

#if PROFILE_FONT_MANAGER
        PUSH("FontManager: load font [name: '%s']", font->getName());
#endif

        font->addReference();
        return font;
    }

    IFont* FontManager::loadFontFromXML(const char *name, XMLNode &node)
    {

    }

    IFont* FontManager::copyFont(IFont *font)
    {
        font->addReference();
        return font;
    }

    IFont* FontManager::getDefaultFont()
    {
        mDefaultFont->addReference();
        return mDefaultFont;
    }

    IFont* FontManager::getDebugFont()
    {
        mDebugFont->addReference();
        return mDebugFont;
    }

    uint32 FontManager::getMemoryUsage()
    {
        return sizeof(FontManager)    +
               mFonts.getMemoryUsage();
    }

    void FontManager::getMemoryUsage(MemorySizer *sizer)
    {
        auto root = sizer->addObject("FontManager", getMemoryUsage());

        for (auto current = mFonts.iterate(); current != nullptr; current = mFonts.next())
        {
            sizer->addChild(root, current->getName(), current->getMemoryUsage());
        }
    }

} // namespace Berserk::Resources