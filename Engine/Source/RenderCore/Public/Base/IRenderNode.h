//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_IRENDERNODE_H
#define BERSERK_IRENDERNODE_H

#include "Misc/Types.h"
#include "Math/AABB.h"
#include "Math/Transform.h"
#include "Base/RenderBase.h"

namespace Berserk
{

    /**
     * Basic class for all 3D/2D space geometry, objects, components, which could
     * be sent to the rendering queue for the following rasterization / image generation
     * by the engine rendering system.
     */
    class IRenderNode : public RenderBase
    {
    public:

        /**
         * Defines the type of the render node element. It affect on the technique,
         * applied to the render node to handle it in the frame rendering pass. Represents
         * all the drawable objects, which could be sent to the rendering queue for rendering.
         */
        enum RenderNodeType : uint32
        {
            eRNT_NOT_RENDER_NODE    = 0,

            eRNT_LIGHT_SOURCE       = 1,    //! Static / Dynamic scene light source
            eRNT_PARTICLE_EMITTER   = 2,    //! Particle emitter object (uses instant or buffered drawing)
            eRNT_VEGETATION         = 3,    //! Optimised vegetation rendering (without self shadowing)
            eRNT_MODEL              = 4,    //! Simple scene model with array of pairs (mesh, material)
            eRNT_TERRAIN            = 5,    //! Wast surface area
            eRNT_WATER_VOLUME       = 6,    //! Water region (ocean, river, sea)
            eRNT_USER_INTERFACE     = 7,    //! 2D user interface elements
            eRNT_HEADS_UP_DISPLAY   = 8,    //! Heads-Up display (with 3D/2D perspective)
            eRNT_STATIC_GEOMETRY    = 9,    //! Static (non movable) model (set of models)
            eRNT_RESERVED_0         = 10,   //! Reserved!
            eRNT_RESERVED_1         = 10,   //! Reserved!
            eRNT_RESERVED_2         = 10,   //! Reserved!
            eRNT_RESERVED_3         = 10,   //! Reserved!
            eRNT_RESERVED_4         = 10,   //! Reserved!
            eRNT_RESERVED_5         = 10,   //! Reserved!

            eRNT_TOTAL_TYPES
        };

        /**
         * Defines additional behaviour for rendering node, which affects on
         * the appearance of the node in the finally rendered image.
         */
        enum RenderNodeFlag : uint32
        {
            eRNF_NO_CULLING     = SHIFT(1),    //! Do not apply frustum culling for this node
            eRNF_CAST_SHADOWS   = SHIFT(2),    //! This node can cast/produce shadows
            eRNF_WIRE_FRAME     = SHIFT(3),    //! Render as wire frame with basic coloring
            eRNF_SHOW_MESH      = SHIFT(4),    //! Show mesh on top of rendered surface
            eRNF_SHOW_AABB      = SHIFT(5),    //! Show axis aligned bounding box of this node
            eRNF_APPLY_LOD      = SHIFT(6),    //! Apply tessellation / level of detail for this node
            eRNF_RESERVED_0     = SHIFT(7),    //! Reserved!
            eRNF_RESERVED_1     = SHIFT(7),    //! Reserved!
            eRNF_RESERVED_2     = SHIFT(7),    //! Reserved!
            eRNF_RESERVED_3     = SHIFT(7),    //! Reserved!
            eRNF_RESERVED_4     = SHIFT(7),    //! Reserved!
            eRNF_RESERVED_5     = SHIFT(7),    //! Has the lowest priority

        };

    public:

        IRenderNode() : mRenderNodeType(eRNT_NOT_RENDER_NODE),
                        mMaxViewDistance(0.0f),
                        mLodRatio(0.0f),
                        mLodLevels(0),
                        mRenderNodeFlags(0)
        {

        }

        virtual ~IRenderNode() = default;

        /**
         * Set chosen render node flag active
         * @param flag   Flag enum to modify
         * @param active True to set active, otherwise false
         */
        virtual void setRenderNodeFlag(RenderNodeFlag flag, bool active);

        /**
         * Set all the render node drawing flags
         * @param mask Render Node flags stored as a bit mask
         */
        virtual void setRenderNodeFlags(uint32 mask) = 0;

        /**
         * Copy debug info about node into buffer
         * @param[out] buffer Buffer to store dubug info
         */
        virtual void getRenderNodeDebugInfo(char* buffer) = 0;

        /** @return Node name */
        virtual const char* getRenderNodeName() = 0;

        /** @return AABB of node for frustum culling */
        virtual const AABB& getAABB() const = 0;

        /** @return Vec3f of 3D space position */
        virtual const Vec3f& getPosition() const = 0;

        /** @return Flags of the node */
        virtual uint32 getRenderNodeFlags() const           { return mRenderNodeFlags; }

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

        /** @return Number of loads for node (used if active flag eRNF_APPLY_LOD) */
        virtual uint32 getLodLevels() const                 { return mLodLevels; }

        /** @return Max view distance from camera, after that node is not visible */
        virtual float32 getMaxViewDistance() const          { return mMaxViewDistance; }

        /** @return Type of this node */
        virtual RenderNodeType getRenderNodeType() const    { return mRenderNodeType; }

    protected:

        RenderNodeType mRenderNodeType;

        float32 mMaxViewDistance;
        float32 mLodRatio;
        uint32  mLodLevels;
        uint32  mRenderNodeFlags;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERNODE_H