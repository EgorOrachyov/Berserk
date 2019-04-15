//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Queue/RenderQueue.h"

namespace Berserk::Render
{

    RenderQueue::RenderQueue(IAllocator* allocator)
            : mSubmitQueue(INITIAL_SUBMIT_QUEUE_SIZE, allocator),
              mRenderQueue(INITIAL_RENDER_QUEUE_SIZE, allocator)
    {
        PUSH("RenderQueue: initialize");
    }

    RenderQueue::~RenderQueue()
    {
        PUSH("RenderQueue: de-initialize");
    }

    void RenderQueue::submit(const RenderQueueNode &node)
    {
        mSubmitQueue += node;
    }

} // namespace Berserk::RenderSystem