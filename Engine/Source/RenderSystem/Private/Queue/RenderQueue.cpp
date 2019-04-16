//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Queue/RenderQueue.h"

namespace Berserk::Render
{

    RenderQueue::RenderQueue(IAllocator* allocator)
            : mSubmitQueue(INITIAL_SUBMIT_QUEUE_SIZE, allocator),
              mAllocator(allocator)
    {
        mRenderQueue = (ArrayList<RenderQueueNode>*) allocator->allocate(sizeof(ArrayList<RenderQueueNode>) * eRQT_TOTAL_TYPES);

        for (uint32 i = 0; i < eRQT_TOTAL_TYPES; i++)
        {
            new(&mRenderQueue[i]) ArrayList<RenderQueueNode>(INITIAL_RENDER_QUEUE_SIZE, allocator);
        }

        PUSH("RenderQueue: initialize");
    }

    RenderQueue::~RenderQueue()
    {
        for (uint32 i = 0; i < eRQT_TOTAL_TYPES; i++)
        {
            delete(&mRenderQueue[i]);
        }

        mAllocator->free(mRenderQueue);

        PUSH("RenderQueue: de-initialize");
    }

    void RenderQueue::submit(const SubmitQueueNode &node)
    {
        mSubmitQueue += node;
    }

    void RenderQueue::submit(const RenderQueueNode &node)
    {
        mRenderQueue[node.mRootComponent->getRenderQueueType()] += node;
    }

    void RenderQueue::update()
    {
        mSubmitQueue.reset();

        for (uint32 i = 0; i < eRQT_TOTAL_TYPES; i++)
        {
            mRenderQueue[i].reset();
        }
    }

} // namespace Berserk::RenderSystem