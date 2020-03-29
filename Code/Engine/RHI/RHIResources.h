/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCES_H
#define BERSERK_RHIRESOURCES_H

#include <RHI/RHIDefinitions.h>

namespace Berserk {

    class RHIResource {
    public:
        virtual ~RHIResource() = default;
    };

    class RHIShader : public RHIResource {
    public:
        ~RHIShader() override = default;
    };

    class RHIVertexLayout : public RHIResource {
    public:
        ~RHIVertexLayout() override = default;
    };

    class RHIVertexBuffer : public RHIResource {
    public:
        ~RHIVertexBuffer() override = default;
    };

    class RHIIndexBuffer : public RHIResource {
    public:
        ~RHIIndexBuffer() override = default;
    };

    class RHIUniformLayout : public RHIResource {
    public:
        ~RHIUniformLayout() override = default;
    };

    class RHIUniformBuffer : public RHIResource {
    public:
        ~RHIUniformBuffer() override = default;
    };

    class RHIUniformSet : public RHIResource {
    public:
        ~RHIUniformSet() override = default;
    };

    class RHITexture : public RHIResource {
    public:
        ~RHITexture() override = default;
    };

    class RHISampler : public RHIResource {
    public:
        ~RHISampler() override = default;
    };

    class RHIFramebuffer : public RHIResource {
    public:
        ~RHIFramebuffer() override = default;
    };

    class RHIGraphicsPipeline : public RHIResource {
    public:
        ~RHIGraphicsPipeline() override = default;
    };

    class RHIDrawList : public RHIResource {
    public:
        ~RHIDrawList() override = default;
    };

    class RHITimeQuery : public RHIResource {
    public:
        ~RHITimeQuery() override = default;
    };

}

#endif //BERSERK_RHIRESOURCES_H
