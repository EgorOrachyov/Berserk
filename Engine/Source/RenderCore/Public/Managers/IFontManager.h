//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_IFONTMANAGER_H
#define BERSERK_IFONTMANAGER_H

#include <XMLDocument.h>
#include <MemorySizer.h>
#include <Foundation/IFont.h>

namespace Berserk::Resources
{

    /**
     * Manages all the fonts loaded in th engine in runtime mode
     */
    class ENGINE_API IFontManager
    {
    public:

        /** De-initialize manager */
        virtual ~IFontManager() = default;

        /** Rename chosen font with new name */
        virtual void renameFont(IFont* font, const char* name) = 0;

        /** Delete specified resource */
        virtual void deleteFont(IFont* font) = 0;

        /** @return Font with specified name */
        virtual IFont* findFont(const char* name) = 0;

        /** @return Pointer to resource with incrementing reference count */
        virtual IFont* getFont(const char* name) = 0;

        /** @return Font from .mtl file with specified name (in path) */
        virtual IFont* loadFont(const char* path) = 0;

        /** @return Loaded font with specified name from XML node */
        virtual IFont* loadFontFromXML(const char* name, XMLNode& node) = 0;

        /** @return Copy (+1 reference) of specified font */
        virtual IFont* copyFont(IFont* font) = 0;

        /** @return Default font for engine */
        virtual IFont* getDefaultFont() = 0;

        /** @return Default debug mono font */
        virtual IFont* getDebugFont() = 0;

        /** @return Memory usage on CPU (RAM) side */
        virtual uint32 getMemoryUsage() = 0;

        /** Profile memroy usage by this manager */
        virtual void getMemoryUsage(MemorySizer* sizer) = 0;
        
    };

} // namespace Berserk::Resources

#endif //BERSERK_IFONTMANAGER_H