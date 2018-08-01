//
// Created by Egor Orachyov on 31.07.2018.
//

#ifndef BERSERKENGINE_AMBIENTLIGHT_H
#define BERSERKENGINE_AMBIENTLIGHT_H

#include "Objects/Lights/Light.h"
#include "Components/AmbientLightComponent.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    class ENGINE_EXPORT AmbientLight : public Light
    {
    public:

        AmbientLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~AmbientLight();

        AmbientLightComponent& getAmbientComponent();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        AmbientLightComponent mAmbientComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_AMBIENTLIGHT_H