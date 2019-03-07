//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_ITEXTUREMANAGER_H
#define BERSERK_ITEXTUREMANAGER_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/ITexture.h"
#include "XML/XMLNode.h"

namespace Berserk
{

    /**
     * An interface which provides access to the texture manager implemented in 3D Rendering System
     * for particular platform graphics driver (OpenGL, DirectX, Vulkan, ...).
     * Responsible for creating, loading, saving textures in OS / Engine system.
     * Handles reference counting and memory allocation for resources.
     */
    class ENGINE_API ITextureManager
    {
    public:

        /** Explicit initialization of manager (must be invoked) */
        virtual void initialize() = 0;

        /** De-initialize manager */
        virtual void release() = 0;

        /** Rename chosen texture with new name */
        virtual void renameTexture(ITexture* texture, const char* name) = 0;

        /** Save texture data in chosen path/name */
        virtual void saveTexture(ITexture* texture, const char* path) = 0;

        /** @return New empty texture with specified name */
        virtual ITexture* createTexture(const char* name) = 0;

        /** @return Texture with specified name */
        virtual ITexture* findTexture(const char* name) = 0;

        /** @return Texture from file with specified name (in path) */
        virtual ITexture* loadTexture(const char* path) = 0;

        /** @return Loaded texture with specified name from XML node */
        virtual ITexture* loadTextureFromXML(const char* name, XMLNode& node) = 0;

        /** @return Copy (+1 reference) of specified texture */
        virtual ITexture* copyTexture(ITexture* texture) = 0;

        /** @return Default texture for models */
        virtual ITexture* getDefaultTexture() = 0;

        /** @return Default texture, if fail to load specified texture */
        virtual ITexture* getDefaultHelperTexture() = 0;

        /** @return Memory usage on CPU (RAM) side */
        virtual uint32 getMemoryUsage() = 0;

        /** @return Memory usage on GPU (VRAM) side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITEXTUREMANAGER_H