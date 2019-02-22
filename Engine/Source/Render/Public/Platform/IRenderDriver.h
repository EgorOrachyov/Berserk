//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IRENDERDRIVER_H
#define BERSERK_IRENDERDRIVER_H

#include "Misc/Types.h"
#include "Math/MathInclude.h"
#include "Platform/IWindow.h"

namespace Berserk
{

    class IRenderDriver
    {
    public:

        struct ViewPort
        {
            float32 startX, startY;
            float32 width, height;
        };

        struct RenderState
        {
            uint32 primitiveType;
            uint32 faceCulling;
            uint32 drawFunc;

            uint32 blendFuncSource;
            uint32 blendFuncDestination;

            uint32 stencilOpFail;
            uint32 stencilOpDepthFail;
            uint32 stencilOpPass;

            uint32 windingOrder;

            Vec3f clearColor;
            ViewPort viewPort;

            bool useDepthTest;
            bool useStencilTest;
            bool useScissorTest;
            bool useAlphaBlending;
        };

    public:

        virtual void initialize(IWindow::WindowSetup& setup) = 0;

        virtual void release() = 0;

        virtual void clear(bool color, bool depth, bool stencil) = 0;

        virtual void setup(const RenderState& state) = 0;

        virtual void swapBuffers() = 0;

        virtual void setActive(IWindow* window) = 0;

        virtual const IWindow* getMainWindow() = 0;

        virtual const RenderState* getCurrentState() = 0;

        virtual const char* getName() = 0;

        virtual const char* getInfo() = 0;

        virtual const char* getShaderInfo() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERDRIVER_H