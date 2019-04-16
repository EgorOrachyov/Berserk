//
// Created by Egor Orachyov on 22.02.2019.
//

#ifndef BERSERK_RENDERQUEUE_H
#define BERSERK_RENDERQUEUE_H

#include <Misc/Types.h>
#include <Memory/IAllocator.h>
#include <Foundation/Forward.h>
#include <Containers/ArrayList.h>
#include <Queue/SubmitQueueNode.h>
#include <Queue/RenderQueueNode.h>
#include <Queue/RenderQueueType.h>
#include <Components/IPrimitiveComponent.h>

namespace Berserk::Render
{
    using namespace Berserk::Engine;

    /**
     * Render queue which stores all the nodes to be rendered in the
     * current frame. Also provides additional buffers for storing sorted
     * and culled nodes.
     *
     * Supports several groups of nodes:
     * - background
     * - geometry
     * - alpha blend
     * - overlay
     *
     * Evolves as storage of the all geometry, which will be processed by the Render Pipeline.
     */
    class ENGINE_API RenderQueue
    {
    public:

        GENARATE_NEW_DELETE(RenderQueue);

        /** Allocator for internal buffers usage */
        explicit RenderQueue(IAllocator* allocator);

        /** Do nothing */
        ~RenderQueue();

        /** Submit node to be culled before rendering */
        void submit(const SubmitQueueNode& node);

        /** Node to draw without frustum culling */
        void submit(const RenderQueueNode& node);

        /** Size of the submit queue to preallocate */
        static const uint32 INITIAL_SUBMIT_QUEUE_SIZE = /* for debug */ 100;

        /** Size of the render queue to preallocate */
        static const uint32 INITIAL_RENDER_QUEUE_SIZE = /* for debug */ 50;

    protected:

        friend class RenderSystem;

        /** First queue to submit all the render nodes */
        ArrayList<SubmitQueueNode> mSubmitQueue;

        /** For buffers allocation */
        class IAllocator* mAllocator;

        /** Store render nodes in groups to render */
        class ArrayList<RenderQueueNode>* mRenderQueue;

    };

} // namespace Berserk::RenderSystem

#endif //BERSERK_RENDERQUEUE_H

