//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_RENDERDRIVER_H
#define BERSERKENGINE_RENDERDRIVER_H

#include "Math/UtilityVectors.h"
#include "Config/ConfigTable.h"

namespace Berserk
{
    class RenderDriver
    {
    public:

        virtual ~RenderDriver() = default;

        virtual void init(const ConfigTable& table) = 0;
        virtual void destroy() = 0;

        virtual void clearColorBuffer(const Vector3f &color) = 0;
        virtual void clearColorBuffer(const Vector4f &color) = 0;

        virtual void clearBuffer() = 0;
        virtual void clearColorBuffer() = 0;
        virtual void clearDepthBuffer() = 0;
        virtual void clearStencilBuffer() = 0;

        virtual void setDefaultBuffer() = 0;
        virtual void setClearColor(const Vector3f &color) = 0;
        virtual void setClearColor(const Vector4f &color) = 0;

        virtual void setWindingOrderCW() = 0;
        virtual void setWindingOrderCCW() = 0;

        virtual void setFrontCulling() = 0;
        virtual void setBackCulling() = 0;

        virtual void enableDepthTest(bool flag) = 0;
        virtual void enableFaceCulling(bool flag) = 0;

    };

    extern RenderDriver* gRenderDriver;

} // namespace Berserk

#endif //BERSERKENGINE_RENDERDRIVER_H