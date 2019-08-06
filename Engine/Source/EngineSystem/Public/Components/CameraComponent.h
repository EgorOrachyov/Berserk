//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_CAMERACOMPONENT_H
#define BERSERK_CAMERACOMPONENT_H

#include <Components/CameraBase.h>
#include <Components/Component.h>

namespace Berserk
{

    class ENGINE_API CameraComponent : public Component, public CameraBase
    {
    public:

        REFLECTABLE_OBJECT(CameraComponent);

    public:

#if DEBUG
        CameraComponent() : Component() { }
#endif

    };

} // namespace Berserk

#endif //BERSERK_CAMERACOMPONENT_H