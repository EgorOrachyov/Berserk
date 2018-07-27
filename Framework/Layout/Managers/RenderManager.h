//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_RENDERMANAGER_H
#define BERSERKENGINE_RENDERMANAGER_H

#include "Objects/Lights/SpotLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/DirectionalLight.h"

namespace Berserk
{

    enum LightInfo
    {
        LI_MAX_SPOT_LIGHTS = 32,
        LI_MAX_POINT_LIGHTS = 32,
        LI_MAX_DIRECTIONAL_LIGHTS = 32
    };

    class RenderManager
    {
    public:

        RenderManager();
        ~RenderManager();

        void queueLight(SpotLight *light);
        void queueLight(PointLight *light);
        void queueLight(DirectionalLight *light);

    private:

        ArrayList<SpotLight*> mSpotLights;
        ArrayList<PointLight*> mPointLights;
        ArrayList<DirectionalLight*> mDirectionalLights;

    };

} // namespace Berserk

#endif //BERSERKENGINE_RENDERMANAGER_H