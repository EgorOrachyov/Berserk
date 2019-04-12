//
// Created by Egor Orachyov on 12.04.2019.
//

#include "Foundation/PipelineScheduler.h"

namespace Berserk::Render
{

    PipelineScheduler::PipelineScheduler(IAllocator *allocator) : mStages(INITIAL_STAGES_COUNT, allocator)
    {
        FAIL(allocator, "Null pointer allocator");
    }

    void PipelineScheduler::addStage(IPipelineStage *stage)
    {
        mStages += stage;
    }

    void PipelineScheduler::execute(RenderPassInfo &passInfo)
    {
        for (uint32 i = 0; i < mStages.getSize(); i++)
        {
            mStages[i]->execute(passInfo);
        }
    }

} // namespace Berserk::Render