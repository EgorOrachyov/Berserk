//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_RENDERBASE_H
#define BERSERK_RENDERBASE_H

#include <Managers/IBufferManager.h>
#include <Managers/IShaderManager.h>
#include <Managers/ITextureManager.h>
#include <Managers/IMaterialManager.h>
#include <Managers/MaterialManager.h>
#include <Managers/FontManager.h>
#include <Managers/DebugDrawManager.h>
#include <Importers/IImageImporter.h>
#include <Importers/IFontImporter.h>
#include <Platform/IWindow.h>
#include <Platform/IRenderDriver.h>
#include <Foundation/IRenderSystem.h>

namespace Berserk::Render
{
    using namespace Resources;
    using namespace Importers;

    /**
     * Basic class for all Rendering Engiene components, which provides centralized
     * access to all 3D Engine managers and systems.
     *
     * Note: each pointer (or link) will be initialized by the Rendering System,
     * which is THE MAIN component of the engine rendering system.
     */
    class ENGINE_API RenderBase
    {
    public:

        /** @return 3D Engine Main Rendering system pointer */
        static IRenderSystem* getRenderSystem()             { return mRenderSystem; }

        /** @return 3D Engine Main Rendering window pointer */
        static IWindow* getMainWindow()                     { return mMainWindow; }

        /** @return 3D Engine Rendering driver pointer */
        static IRenderDriver* getRenderDriver()             { return mRenderDriver; }

        /** @return 3D Engine BufferManager pointer */
        static IBufferManager* getBufferManager()           { return mBufferManager; }

        /** @return 3D Engine ShaderManager pointer */
        static IShaderManager* getShaderManager()           { return mShaderManager; }

        /** @return 3D Engine TextureManager pointer */
        static ITextureManager* getTextureManager()         { return mTextureManager; }

        /** @return 3D Engine MaterialManager pointer */
        static IMaterialManager* getMaterialManager()       { return mMaterialManager; }

        /** @return 3D Engine DebugDrawManager pointer */
        static DebugDrawManager* getDebugRenderManager()    { return mDebugDrawManager; }

        /** @return 3D Engine ImageImporter pointer */
        static IImageImporter* getImageImporter()           { return mImageImporter; }

        /** @return 3D Engine FontImporter pointer */
        static IFontImporter* getFontImporter()             { return mFontImporter; }

        /** @return 3D Engine FontManager pointer */
        static IFontManager* getFontManager()               { return mFontManager; }

    protected:

        friend class RenderSystem;

        static class IRenderSystem* mRenderSystem;

        static class IWindow* mMainWindow;

        static class IRenderDriver* mRenderDriver;

        static class IBufferManager* mBufferManager;

        static class IShaderManager* mShaderManager;

        static class ITextureManager* mTextureManager;

        static class IMaterialManager* mMaterialManager;

        static class DebugDrawManager* mDebugDrawManager;

        static class IImageImporter* mImageImporter;

        static class IFontImporter* mFontImporter;

        static class IFontManager* mFontManager;

    };

} // namespace Berserk

#endif //BERSERK_RENDERBASE_H