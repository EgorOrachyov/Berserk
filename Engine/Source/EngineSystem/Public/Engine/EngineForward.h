//
// Created by Egor Orachyov on 2019-09-01.
//

#ifndef BERSERK_ENGINEFORWARD_H
#define BERSERK_ENGINEFORWARD_H

namespace Berserk
{

    template <typename T>
    class ResourceHandle;

    class Font;
    class Mesh;
    class Image;
    class Texture;
    class Texture2D;
    class Material;

    typedef ResourceHandle<Font> HFort;
    typedef ResourceHandle<Mesh> HMesh;
    typedef ResourceHandle<Image> HImage;
    typedef ResourceHandle<Texture> HTexture;
    typedef ResourceHandle<Texture2D> HTexture2D;
    typedef ResourceHandle<Material> HMaterial;

    template <typename T>
    class ObjectHandle;

    class Object;
    class Component;
    class CameraComponent;
    class LightComponent;
    class SpotLightComponent;
    class PointLightComponent;
    class DirectionalLightComponent;
    class RenderComponent;
    class MeshComponent;
    class StaticMeshComponent;

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

}

#endif //BERSERK_ENGINEFORWARD_H
