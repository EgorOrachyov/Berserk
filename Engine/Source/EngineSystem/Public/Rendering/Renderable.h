//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_RENDERABLE_H
#define BERSERK_RENDERABLE_H

#include <Engine/EngineResources.h>

namespace Berserk
{

    /**
     * Render data, shared between main and render thread. Any visible
     * object on the render scene is presented via this class. Other render object
     * properties are stored in render component and copied via proxy.
     *
     * @note Must stay immutable in time of thread sharing
     */
    class ENGINE_API Renderable final : public Allocatable
    {
    public:

        Renderable(const MeshHandle &mesh,
                   const TArray<MaterialHandle> &materials,
                   const TArray<AABB> &bounds,
                   const TArray<Mat4x4f> &transformations)
           : mMesh(mesh),
             mMaterials(materials),
             mBounds(bounds),
             mTransformations(transformations)
        {

        }

        Renderable(const Renderable& renderable) = default;

        Renderable(Renderable&& renderable) = default;

        ~Renderable() = default;

        /** @return Mesh of this render object */
        const MeshHandle& getMesh() const { return mMesh; }

        /** @return Materials used by mesh nodes of this render object */
        const TArray<MaterialHandle> &getMaterials() const { return mMaterials; }

        /** @return Bounds used for culling of this renderable */
        const TArray<AABB> &getBounds() const { return mBounds; }

        /** @return Transformations of mesh nodes of this render object */
        const TArray<Mat4x4f> &getTransformations() const { return mTransformations; }

    private:

        MeshHandle mMesh;
        TArray<MaterialHandle> mMaterials;
        TArray<AABB> mBounds;
        TArray<Mat4x4f> mTransformations;

    };

} // namespace Berserk

#endif //BERSERK_RENDERABLE_H
