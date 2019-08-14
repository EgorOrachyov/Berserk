//
// Created by Egor Orachyov on 2019-08-08.
//

#include "Rendering/RenderQueue.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    RenderQueue::RenderQueue(ERenderQueueSortOptions options, uint32 nodesCount, IAllocator &allocator)
        : mSortOptions(options),
          mQueue(nodesCount, allocator)
    {

    }

    void RenderQueue::add(const RenderElement& element, float32 viewDistance)
    {
        // todo: add material ID
        mQueue.emplace(element, viewDistance, 0x0);
    }

    void RenderQueue::clear()
    {
        /** Optimize: no complex data */
        mQueue.clearNoDestructorCall();
    }

    void RenderQueue::sort()
    {
        SortOperation operation = nullptr;

        switch (mSortOptions)
        {
            case ERenderQueueSortOptions::RQSO_Distance:
                operation = sortDistance;
                break;
            case ERenderQueueSortOptions::RQSO_Alpha:
                operation = sortAplha;
                break;
            default:
                operation = sortDistance;
                DEBUG_LOG_ERROR("RenderQueue: unknown sort option type");
        }

        mQueue.sort(operation);
    }

} // namespace Berserk