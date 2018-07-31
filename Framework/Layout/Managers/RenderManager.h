//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_RENDERMANAGER_H
#define BERSERKENGINE_RENDERMANAGER_H

#include "Containers/List.h"
#include "Objects/GPU/GPUBuffer.h"
#include "Components/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AmbientLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/BaseMaterialComponent.h"

namespace Berserk
{

    enum LightInfo
    {
        LI_MAX_SPOT_LIGHTS          = 16,
        LI_MAX_POINT_LIGHTS         = 16,
        LI_MAX_DIRECTIONAL_LIGHTS   = 16
    };

    struct BaseRenderMaterialComponent
    {
    public:
        BaseRenderMaterialComponent(Matrix4x4f *matrix4x4f, GPUBuffer *buffer, BaseMaterialComponent *materialComponent);

        Matrix4x4f *mTransformationComponent;
        GPUBuffer  *mRenderBufferComponent;
        BaseMaterialComponent *mMaterialComponent;
    };

    class RenderManager
    {
    public:

        RenderManager();
        ~RenderManager();

        void update();

        void queueCamera(CameraComponent *camera);
        void queueLight(AmbientLightComponent *light);
        void queueLight(SpotLightComponent *light);
        void queueLight(PointLightComponent *light);
        void queueLight(DirectionalLightComponent *light);

        void queueBuffer(GPUBuffer *buffer);
        void queueTransformation(Matrix4x4f *matrix4x4f);
        void queueMaterial(BaseMaterialComponent *materialComponent);

        const CameraComponent *getCamera() const;
        const List<SpotLightComponent*> &getSpotLights() const;
        const List<PointLightComponent*> &getPointLights() const;
        const List<DirectionalLightComponent*> &getDirectionalLights() const;
        const List<BaseRenderMaterialComponent> &getBaseRenderMaterials() const;

    private:

        GPUBuffer *mTmpBuffer;
        Matrix4x4f *mTmpMatrix4x4f;

        CameraComponent *mCamera;                               // Defines camera which will be used for next rendering

        List<SpotLightComponent*> mSpotLights;                  // Defines spot light sources for next rendering
        List<PointLightComponent*> mPointLights;                // Defines point light sources for next rendering
        List<DirectionalLightComponent*> mDirectionalLights;    // Defines directional light sources for next rendering

        List<BaseRenderMaterialComponent> mBaseRenderMaterials; // Defines pairs of materials and gpu buffers for rendering pass

    };

} // namespace Berserk

#endif //BERSERKENGINE_RENDERMANAGER_H