/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERTARGETPROXY_H
#define BERSERK_RENDERTARGETPROXY_H

#include <Rendering/IRenderTarget.h>
#include <TPtrShared.h>
#include <Math/Vec2f.h>

namespace Berserk {
    namespace Rendering {

        /** How resized child render targets (proxy) */
        enum class ERenderTargetResizePolicy : uint32 {
            AreaFit,
            AreaProportional
        };

        class RenderTargetProxy : public IRenderTarget, public IRenderTargetResizeListener {
        public:
            RenderTargetProxy(TPtrShared<IRenderTarget> target, float posScale, float sizeScale);
            RenderTargetProxy(TPtrShared<IRenderTarget> target, Size2i relativePos, Size2i size);
            ~RenderTargetProxy() override;

            void onResized(const Region2i &oldArea, const Region2i &newArea) override;
            void bind(RHIDrawList &drawList) const override;

            bool isScreenTarget() const override;
            bool isTextureTarget() const override;
            bool isProxyTarget() const override;

            const TPtrShared<IRenderTarget> &getRootTarget() const { return mRootTarget; }

            const Vec2f &getProportionPos() const { return mProportionPos; }
            const Vec2f &getProportionSize() const { return mProportionPos; }

            ERenderTargetResizePolicy getResizePolicy() const { return mResizePolicy; }

        private:
            void updateSize(const Region2i &oldArea, const Region2i &newArea);

            TPtrShared<IRenderTarget> mRootTarget;
            Vec2f mProportionPos = Vec2f(0, 0);
            Vec2f mProportionSize = Vec2f(1, 1);
            ERenderTargetResizePolicy mResizePolicy = ERenderTargetResizePolicy::AreaFit;
        };

    }
}

#endif //BERSERK_RENDERTARGETPROXY_H