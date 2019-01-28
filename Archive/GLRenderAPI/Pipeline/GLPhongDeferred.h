//
// Created by Egor Orachyov on 19.08.2018.
//

#ifndef BERSERKENGINE_GLPHONGDEFERRED_H
#define BERSERKENGINE_GLPHONGDEFERRED_H

#include "Render/PipelineStage.h"
#include "GPUProgram/GLGPUProgram.h"
#include "Uniform/UniformInfo.h"

namespace Berserk
{

    class GLPhongDeferred : public PipelineStage
    {
    private:

        class UniformData
        {
        public:

            INT32 CameraPosition;
            INT32 AmbientLight;

            INT32 gPosition;
            INT32 gNormal;
            INT32 gDiffuse;
            INT32 gSpecularSh;
            INT32 gSSAO;

            UDirectLight    dirLight    [LightInfo::LI_MAX_DIRECTIONAL_LIGHTS];
            USpotLight      spotLight   [LightInfo::LI_MAX_SPOT_LIGHTS];
            UPointLight     pointLight  [LightInfo::LI_MAX_POINT_LIGHTS];

            UDirectLight    dirSLight   [ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
            USpotLight      spotSLight  [ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];
            UPointLight     pointSLight [ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES];

            INT32 dirMap    [ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
            INT32 spotMap   [ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];
            INT32 pointMap  [ShadowInfo::SI_MAX_POINT_SHADOW_SOURCES];

            INT32 dirMat    [ShadowInfo::SI_MAX_DIR_SHADOW_SOURCES];
            INT32 spotMat   [ShadowInfo::SI_MAX_SPOT_SHADOW_SOURCES];

            INT32 NUM_DIR_L;
            INT32 NUM_SPOT_L;
            INT32 NUM_POINT_L;

            INT32 NUM_DIR_SL;
            INT32 NUM_SPOT_SL;
            INT32 NUM_POINT_SL;

            UINT32 SSAOPass_Use;
            UINT32 SSAOPass_NoUse;
        };

    public:

        virtual ~GLPhongDeferred() = default;

        void init() override;
        void destroy() override;
        void execute() override;

    private:

        UniformData  mUniform;
        GLGPUProgram mProgram;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLPHONGDEFERRED_H