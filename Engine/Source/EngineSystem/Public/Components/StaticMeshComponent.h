//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_STATICMESHCOMPONENT_H
#define BERSERK_STATICMESHCOMPONENT_H

#include <Components/MeshComponent.h>
#include <Components/IPrimitiveComponent.h>

namespace Berserk::Engine
{
    using namespace Resources;

    /**
     * Default rendering primitive. Has transformation, rendering properties.
     * Consists of pairs of material and geometry buffer. Allows render static
     * models without any kind of animations.
     */
    class ENGINE_EXPORT StaticMeshComponent : public IPrimitiveComponent
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(StaticMeshComponent);

        /** Default object setup via initializer */
        explicit StaticMeshComponent(const IObjectInitializer& objectInitializer);

        /** Free Render System resources */
        ~StaticMeshComponent() override;

    public:

    #if DEBUG

        /**
         * Adds new mesh from raw buffer and material data
         * @warning Debug purpose only
         * @note Reference count to used resource will decremented in destructor
         */
        void addRawData(IGPUBuffer* buffer, IMaterial* material, void* data);

    #endif

        uint32 getNumOfMaterials() override             { return mUsedMaterials.getSize(); }

        void setBoundingBox(const AABB &box) override   { mBoundingBox = box; }

        const AABB& getBoundingBox() override           { return mBoundingBox; }

    protected:

        static const uint32 INITIAL_MESH_COUNT = 4;

        static const uint32 INITIAL_MATERIAL_COUNT = 4;

        /** Pairs of material and geometry */
        ArrayList<MeshComponent> mSubMeshSet;

        /** Raw data of mesh components */
        ArrayList<void*> mSourceData;

        /** All the materials used by this mesh */
        ArrayList<IMaterial*> mUsedMaterials;

        /** Bounds all the attached mesh components */
        AABB mBoundingBox;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_STATICMESHCOMPONENT_H