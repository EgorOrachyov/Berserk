//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLRENDERDRIVER_H
#define BERSERKENGINE_GLRENDERDRIVER_H

#include "Render/RenderDriver.h"
#include "Math/MathInclude.h"

namespace Berserk
{

    class GLRenderDriver : public RenderDriver
    {
    public:

        virtual ~GLRenderDriver() = default;

        void init(const ConfigTable& table) override;
        void destroy() override;

        void clearColorBuffer(const Vector3f &color) override;
        void clearColorBuffer(const Vector4f &color) override;

        void clearBuffer() override;
        void clearColorBuffer() override;
        void clearDepthBuffer() override;
        void clearStencilBuffer() override;

        void setDefaultBuffer() override;
        void setClearColor(const Vector3f &color) override;
        void setClearColor(const Vector4f &color) override;
        void setViewPort(UINT32 x, UINT32 y, UINT32 width, UINT32 height) override;

        void setWindingOrderCW() override;
        void setWindingOrderCCW() override;

        void setFrontCulling() override;
        void setBackCulling() override;

        void enableDepthTest(bool flag) override;
        void enableFaceCulling(bool flag) override;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLRENDERDRIVER_H