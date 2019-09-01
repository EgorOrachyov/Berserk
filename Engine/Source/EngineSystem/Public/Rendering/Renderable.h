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
     * @note Must stay immutable in time of threaded execution
     */
    class ENGINE_API Renderable final : public Allocatable
    {
    public:

        Renderable(IAllocator& allocator = Allocator::get())
            : mMaterials(allocator),
              mBounds(allocator),
              mTransformations(allocator)
        {

        }

        Renderable(HMesh mesh,
                   TArray<HMaterial> &materials,
                   TArray<AABB> &bounds,
                   TArray<Mat4x4f> &transformations)
           : mMesh(std::move(mesh)),
             mMaterials(std::move(materials)),
             mBounds(std::move(bounds)),
             mTransformations(std::move(transformations))
        {

        }

        /** Sets mesh of this render object */
        void setMesh(const HMesh& mesh) { mMesh = mesh; }

        /** @return Mesh of this render object */
        HMesh& getMesh() { return mMesh; }

        /** @return Materials used by mesh nodes of this render object */
        TArray<HMaterial> &getMaterials() { return mMaterials; }

        /** @return Bounds used for culling of this renderable */
        TArray<AABB> &getBounds() { return mBounds; }

        /** @return Transformations of mesh nodes of this render object */
        TArray<Mat4x4f> &getTransformations() { return mTransformations; }

    private:

        /** Mesh to be rendered */
        HMesh mMesh;

        /** Materials list [could be empty] - maps mesh nodes to materials via indices */
        TArray<HMaterial> mMaterials;

        /** Bounds [could be empty - therefore object is visible] */
        TArray<AABB> mBounds;

        /** World transformations list [could be empty] */
        TArray<Mat4x4f> mTransformations;

    };

} // namespace Berserk

#endif //BERSERK_RENDERABLE_H
