//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_RENDERMANAGER_H
#define BERSERKENGINE_RENDERMANAGER_H


#include "Containers/Queue.h"
#include "Components/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/DirectionalLightComponent.h"

namespace Berserk
{

    enum LightInfo
    {
        LI_MAX_SPOT_LIGHTS          = 32,
        LI_MAX_POINT_LIGHTS         = 32,
        LI_MAX_DIRECTIONAL_LIGHTS   = 32
    };

    class RenderManager
    {
    public:

        RenderManager();
        ~RenderManager();

        void update();

        void queueCamera(CameraComponent *camera);
        void queueLight(SpotLightComponent *light);
        void queueLight(PointLightComponent *light);
        void queueLight(DirectionalLightComponent *light);

        const CameraComponent *getCamera() const;
        const Queue<SpotLightComponent*> &getSpotLights() const;
        const Queue<PointLightComponent*> &getPointLights() const;
        const Queue<DirectionalLightComponent*> &getDirectionalLights() const;

    private:

        CameraComponent *mCamera;                                // Defines camera which will be used for next rendering

        Queue<SpotLightComponent*> mSpotLights;                  // Defines spot light sources for next rendering
        Queue<PointLightComponent*> mPointLights;                // Defines point light sources for next rendering
        Queue<DirectionalLightComponent*> mDirectionalLights;    // Defines directional light sources for next rendering


    };

} // namespace Berserk

#endif //BERSERKENGINE_RENDERMANAGER_H