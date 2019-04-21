//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_TONEMAPPING_H
#define BERSERK_TONEMAPPING_H

#include <Platform/IShader.h>
#include <Foundation/IPipelineStage.h>

namespace Berserk::Render
{

    class GRAPHICS_API ToneMapping : public IPipelineStage
    {
    public:

        GENARATE_NEW_DELETE(DebugDraw);

        /** Standard stage init */
        ToneMapping(const char* name, IAllocator* allocator);

        /** Free resources */
        ~ToneMapping() override;

        /** Called when render system setting updated */
        void reload() override;

        /** Called each render frame */
        void execute(RenderPassInfo& passInfo) override;

    private:

        /** Shader used for tone mapping and rendering screen on display */
        class IShader* mScreenRender = nullptr;

        /** Screen plane buffer */
        class IGPUBuffer* mScreenPlane = nullptr;
    };

} // namespace Berserk::Render




#endif //BERSERK_TONEMAPPING_H
