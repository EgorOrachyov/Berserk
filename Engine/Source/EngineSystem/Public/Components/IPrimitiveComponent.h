//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_IPRIMITIVECOMPONENT_H
#define BERSERK_IPRIMITIVECOMPONENT_H

#include <Foundation/RenderBase.h>
#include <Components/SceneComponent.h>

namespace Berserk::Engine
{

    /**
     * Base class for any kind of primitive / geometry, which could be rendered
     * on scene by render system. Allows to transform, enable/disable effects.
     * See also: StaticMeshComponent, MeshComponent.
     */
    class ENGINE_API IPrimitiveComponent : public SceneComponent, public Render::RenderBase
    {
    public:

        /**
         * All types of rendering primitives, which could be
         * rendered in the engine
         */
        enum PrimitiveType
        {
            ePT_NOT_PRIMITIVE = 0,          //! Default value [ignored by render system]
            ePT_STATIC_MESH,                //! Static mesh type id
            ePT_DYNAMIC_MESH,               //! Dynamic mesh type id

            ePT_TOTAL_PRIMITIVE_TYPES
        };

    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(IPrimitiveComponent);

        /** Default object setup via initializer */
        explicit IPrimitiveComponent(const IObjectInitializer& objectInitializer);

        /** Do actually nothing */
        ~IPrimitiveComponent() override = default;

    public:

        /** @return Type of the primitive [used by render system] */
        PrimitiveType getPrimitiveType()    { return mPrimitiveType; }

        /** @return True, if this primitive can cast local shadows */
        bool castShadows() const            { return mCastShadows; }

        /** @return True, if this primitive can cast far shadows for global light */
        bool castFarShadows() const         { return mCastFarShadows; }

        /** @return True, if can use frustum culling for this */
        bool canApplyCulling() const        { return mCanApplyCulling; }

        /** @return True, if need draw wire frame for on top of the primitive */
        bool drawWireFrame() const          { return mDrawWireFrame; }

        /** @return True, if need draw wire frame only */
        bool drawWireFrameOnly() const      { return mDrawWireFrameOnly; }

        /** @return True, if need draw bounding box */
        bool drawBoundingBox() const        { return mDrawBoundingBox; }

    public:

        virtual uint32 getNumOfMaterials() = 0;

        virtual void setBoundingBox(const AABB &box) = 0;

        virtual const AABB& getBoundingBox() = 0;

    protected:

        friend class ::Berserk::Render::RenderSystem;

        /** Type of the primitive */
        PrimitiveType mPrimitiveType;

        /** If this primitive can cast local shadows */
        bool mCastShadows : 1;

        /** If this primitive can cast far shadows for global light */
        bool mCastFarShadows : 1;

        /** Can use frustum culling for this */
        bool mCanApplyCulling : 1;

        /** Draw wire frame for debug purpose on top of the primitive */
        bool mDrawWireFrame : 1;

        /** Draw wire frame only (without filled polygons) */
        bool mDrawWireFrameOnly : 1;

        /** Draw bounding box */
        bool mDrawBoundingBox : 1;

        /** Previous registered component in render system  */
        IPrimitiveComponent* mPrev = nullptr;

        /** Next registered component in render system  */
        IPrimitiveComponent* mNext = nullptr;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IPRIMITIVECOMPONENT_H