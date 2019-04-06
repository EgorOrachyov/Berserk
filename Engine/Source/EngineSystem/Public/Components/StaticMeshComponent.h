//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_STATICMESHCOMPONENT_H
#define BERSERK_STATICMESHCOMPONENT_H

#include <Engine/Mesh.h>
#include <Components/IPrimitiveComponent.h>

namespace Berserk::EngineSystem
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

        /**
         * @warning Debug purpose only
         * Adds raw mesh data to this component
         */
        virtual void addRawData(const Mesh& mesh);

        /**
         * @warning Debug purpose only
         * Adds new mesh from raw buffer and material data
         */
        virtual void addRawData(IGPUBuffer* buffer, IMaterial* material);

    protected:

        static const uint32 INITIAL_MESH_COUNT = 4;

        static const uint32 INITIAL_MATERIAL_COUNT = 4;

        /** Pairs of material and geometry */
        ArrayList<Mesh> mSubMeshSet;

        /** All the materials used by this mesh */
        ArrayList<IMaterial*> mUsedMaterials;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_STATICMESHCOMPONENT_H