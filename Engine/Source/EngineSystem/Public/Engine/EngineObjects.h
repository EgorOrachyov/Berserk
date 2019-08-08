//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_ENGINEOBJECTS_H
#define BERSERK_ENGINEOBJECTS_H

#include <Object/Object.h>
#include <Components/Component.h>
#include <Components/CameraComponent.h>
#include <Components/LightComponent.h>
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/RenderComponent.h>
#include <Components/MeshComponent.h>
#include <Components/StaticMeshComponent.h>

#include <Object/ObjectHandle.h>

namespace Berserk
{

    typedef ObjectHandle<Object> ObjectRef;
    typedef ObjectHandle<Component> ComponentRef;
    typedef ObjectHandle<CameraComponent> CameraComponentRef;
    typedef ObjectHandle<LightComponent> LightComponentRef;
    typedef ObjectHandle<SpotLightComponent> SpotLightComponentRef;
    typedef ObjectHandle<PointLightComponent> PointLightComponentRef;
    typedef ObjectHandle<DirectionalLightComponent> DirectionalLightComponentRef;
    typedef ObjectHandle<RenderComponent> RenderComponentRef;
    typedef ObjectHandle<MeshComponent> MeshComponentRef;
    typedef ObjectHandle<StaticMeshComponent> StaticMeshComponentRef;

} // namespace Berserk

#endif //BERSERK_ENGINEOBJECTS_H
