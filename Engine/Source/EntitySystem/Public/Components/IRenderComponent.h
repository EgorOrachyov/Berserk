//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_IRENDERCOMPONENT_H
#define BERSERK_IRENDERCOMPONENT_H

#include "Misc/Types.h"
#include "Math/AABB.h"
#include "Math/Transform.h"
#include "Base/RenderBase.h"
#include "Components/IEntityComponent.h"

namespace Berserk::EntitySystem
{

    /**
     * Basic class for all 3D/2D space geometry, objects, components, which could
     * be sent to the rendering queue for the following rasterization / image generation
     * by the engine rendering system.
     */
    class ENGINE_API IRenderComponent : public RenderBase, public IEntityComponent
    {
    public:

        /**
         * Defines the type of the render component elements. It affect on the technique,
         * applied to the render node to handle it in the frame rendering pass. Represents
         * all the drawable objects, which could be sent to the rendering queue for rendering.
         */
        enum RenderComponentType : uint32
        {
            eRCT_NOT_RENDER_NODE    = 0,

            eRCT_LIGHT_SOURCE       = 1,    //! Static / Dynamic scene light source
            eRCT_PARTICLE_EMITTER   = 2,    //! Particle emitter object (uses instant or buffered drawing)
            eRCT_VEGETATION         = 3,    //! Optimised vegetation rendering (without self shadowing)
            eRCT_MODEL              = 4,    //! Simple scene model with array of pairs (mesh, material)
            eRCT_TERRAIN            = 5,    //! Wast surface area
            eRCT_WATER_VOLUME       = 6,    //! Water region (ocean, river, sea)
            eRCT_USER_INTERFACE     = 7,    //! 2D user interface elements
            eRCT_HEADS_UP_DISPLAY   = 8,    //! Heads-Up display (with 3D/2D perspective)
            eRCT_STATIC_GEOMETRY    = 9,    //! Static (non movable) model (set of models)
            eRCT_RESERVED_0         = 10,   //! Reserved!
            eRCT_RESERVED_1         = 10,   //! Reserved!
            eRCT_RESERVED_2         = 10,   //! Reserved!
            eRCT_RESERVED_3         = 10,   //! Reserved!
            eRCT_RESERVED_4         = 10,   //! Reserved!
            eRCT_RESERVED_5         = 10,   //! Reserved!

            eRCT_TOTAL_TYPES
        };

        /**
         * Defines additional behaviour for rendering node, which affects on
         * the appearance of the node in the finally rendered image.
         */
        enum RenderComponentFlag : uint32
        {
            eRCF_NO_CULLING     = SHIFT(1),    //! Do not apply frustum culling for this node
            eRCF_CAST_SHADOWS   = SHIFT(2),    //! This node can cast/produce shadows
            eRCF_WIRE_FRAME     = SHIFT(3),    //! Render as wire frame with basic coloring
            eRCF_SHOW_MESH      = SHIFT(4),    //! Show mesh on top of rendered surface
            eRCF_SHOW_AABB      = SHIFT(5),    //! Show axis aligned bounding box of this node
            eRCF_APPLY_LOD      = SHIFT(6),    //! Apply tessellation / level of detail for this node
            eRCF_RESERVED_0     = SHIFT(7),    //! Reserved!
            eRCF_RESERVED_1     = SHIFT(7),    //! Reserved!
            eRCF_RESERVED_2     = SHIFT(7),    //! Reserved!
            eRCF_RESERVED_3     = SHIFT(7),    //! Reserved!
            eRCF_RESERVED_4     = SHIFT(7),    //! Reserved!
            eRCF_RESERVED_5     = SHIFT(7),    //! Has the lowest priority

        };

    public:

        explicit IRenderComponent(const IObjectInitializer& initializer);

        ~IRenderComponent() override = default;

        /**
         * Set chosen render component flag active
         * @param flag   Flag enum to modify
         * @param active True to set active, otherwise false
         */
        virtual void setRenderComponentFlag(RenderComponentFlag flag, bool active);

        /**
         * Set all the render component drawing flags
         * @param mask Render Component flags stored as a bit mask
         */
        virtual void setRenderComponentFlags(uint32 mask)           { mRenderComponentFlags = mask; }

        /**
         * Copy debug info about component into buffer
         * @param[out] buffer Buffer to store dubug info
         */
        virtual void getRenderComponentDebugInfo(char* buffer) = 0;

        /** @return Component name */
        virtual const char* getRenderComponentName() = 0;

        /** @return AABB of component for frustum culling */
        virtual const AABB& getAABB() const = 0;

        /** @return Vec3f of 3D space position */
        virtual const Vec3f& getPosition() const = 0;

        /** @return Flags of the component */
        virtual uint32 getRenderComponentFlags() const              { return mRenderComponentFlags; }

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

        /** @return Number of loads for component (used if active flag eRNF_APPLY_LOD) */
        virtual uint32 getLodLevels() const                         { return mLodLevels; }

        /** @return Max view distance from camera, after that component is not visible */
        virtual float32 getMaxViewDistance() const                  { return mMaxViewDistance; }

        /** @return Type of this component */
        virtual RenderComponentType getRenderComponentType() const  { return mRenderComponentType; }

    protected:

        RenderComponentType mRenderComponentType;

        float32 mMaxViewDistance;
        float32 mLodRatio;
        uint32  mLodLevels;
        uint32  mRenderComponentFlags;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IRENDERCOMPONENT_H