//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_RENDERBASE_H
#define BERSERK_RENDERBASE_H

#include <Managers/IBufferManager.h>
#include <Managers/IShaderManager.h>
#include <Managers/ITextureManager.h>
#include <Managers/IMaterialManager.h>
#include <Managers/IDebugDrawManager.h>

namespace Berserk::Render
{
    using namespace Resources;

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

        /** @return 3D Engine BufferManager pointer */
        static IBufferManager* getBufferManager()           { return mBufferManager; }

        /** @return 3D Engine ShaderManager pointer */
        static IShaderManager* getShaderManager()           { return mShaderManager; }

        /** @return 3D Engine TextureManager pointer */
        static ITextureManager* getTextureManager()         { return mTextureManager; }

        /** @return 3D Engine MaterialManager pointer */
        static IMaterialManager* getMaterialManager()       { return mMaterialManager; }

        /** @return 3D Engine DebugDrawManager pointer */
        static IDebugDrawManager* getDebugRenderManager()  { return mDebugRenderManager; }

    protected:

        static class IBufferManager* mBufferManager;

        static class IShaderManager* mShaderManager;

        static class ITextureManager* mTextureManager;

        static class IMaterialManager* mMaterialManager;

        static class IDebugDrawManager* mDebugRenderManager;

    };

} // namespace Berserk

#endif //BERSERK_RENDERBASE_H