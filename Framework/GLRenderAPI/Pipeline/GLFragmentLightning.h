//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_GLFRAGMENTLIGHTNING_H
#define BERSERKENGINE_GLFRAGMENTLIGHTNING_H

#include "GLRenderPipeline.h"
#include "Managers/RenderManager.h"

namespace Berserk
{

    class GLFragmentLightning : public GLRenderPipeline
    {
    private:

        struct BaseMaterial
        {
        public:
            INT32 Ambient;
            INT32 Diffuse;
            INT32 Specular;
            INT32 Shininess;
        };

        struct SpotLight
        {
        public:
            INT32 Position;
            INT32 Direction;
            INT32 Intensity;

            INT32 cutoff;
            INT32 outerCutoff;
            INT32 epsilon;
            INT32 exponent;
        };

        struct PointLight
        {
        public:
            INT32 Position;
            INT32 Intensity;

            INT32 constant;
            INT32 linear;
            INT32 quadratic;

            INT32 radius;
        };

        struct DirectLight
        {
        public:
            INT32 Direction;
            INT32 Intensity;
        };

        class UniformData
        {
        public:
            INT32 ModelView;
            INT32 MVP;

            INT32 NUM_OF_DIR_LIGHTS;
            INT32 NUM_OF_SPOT_LIGHTS;
            INT32 NUM_OF_POINT_LIGHTS;

            SpotLight spotLights[LightInfo::LI_MAX_SPOT_LIGHTS];
            PointLight pointLights[LightInfo::LI_MAX_POINT_LIGHTS];
            DirectLight directLights[LightInfo::LI_MAX_DIRECTIONAL_LIGHTS];
        };

        class UniformBaseMaterial : public UniformData
        {
        public:
            BaseMaterial Material;
        };

    public:

        GLFragmentLightning();
        virtual ~GLFragmentLightning();

        void init() override;
        void destroy() override;
        GLFrameBufferObject* process(RenderManager *manager, GLFrameBufferObject *object) override;

    private:

        GLGPUProgram mProgram;
        UniformBaseMaterial mUniform;

    };

}

#endif //BERSERKENGINE_GLFRAGMENTLIGHTNING_H