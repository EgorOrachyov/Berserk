//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include "System.h"
#include "Objects/GPU/GPUBuffer.h"

namespace Berserk
{

    class RenderSystem : public System
    {
    public:

        virtual ~RenderSystem() = default;

        virtual void beginRenderPass1() = 0;
        virtual void endRenderPass1() = 0;

        virtual void beginRenderPass2() = 0;
        virtual void endRenderPass2() = 0;

        virtual const CString& getRenderName() const = 0;
        virtual const CString& getShadingLanguageName() const = 0;

        virtual void setViewportBorders(UINT32 cinematic) = 0;
        virtual void setViewportBorders(UINT32 left, UINT32 right, UINT32 bottom, UINT32 top) = 0;

        virtual UINT32 getWindowWidth() const = 0;
        virtual UINT32 getWindowHeight() const = 0;

        virtual GPUBuffer *createGPUBuffer(const CStaticString &name) = 0;

        static RenderSystem &getRenderSystemRef();
        static RenderSystem *getRenderSystemPtr();

    };

    /// Should be initialized via Application Context
    extern RenderSystem *gRenderSystem;

} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H
