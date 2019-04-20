//
// Created by Egor Orachyov on 19.04.2019.
//

#ifndef BERSERK_DEBUGDRAW_H
#define BERSERK_DEBUGDRAW_H

#include <Platform/IShader.h>
#include <Foundation/IPipelineStage.h>

namespace Berserk::Render
{
    class GRAPHICS_API DebugDraw : public IPipelineStage
    {
    public:

        GENARATE_NEW_DELETE(DebugDraw);

        /** Standard stage init */
        DebugDraw(const char* name, IAllocator* allocator);

        /** Free resources */
        ~DebugDraw() override;

        /** Called when render system setting updated */
        void reload() override;

        /** Called each render frame */
        void execute(RenderPassInfo& passInfo) override;

    private:

        static const uint32 TEXT_BUFFER_COUNT = Buffers::KiB;

        /** Shader used to draw 2D text on screen */
        class IShader* mTextRender = nullptr;

        /** Default debug font */
        class IFont* mDebugFont = nullptr;

        /** Streamed storage for text data */
        class IGPUBuffer* mTextBuffer = nullptr;

    };

} // namespace Berserk::Render

#endif //BERSERK_DEBUGDRAW_H
