//
// Created by Egor Orachyov on 2019-08-22.
//

#include "PipelineStageManager.h"

namespace Berserk
{

    PipelineStageManager::PipelineStageManager(uint32 stages, IAllocator &allocator)
        : mStagesList(stages, allocator)
    {

    }

    void PipelineStageManager::addStage(const Berserk::PipelineStageRef &stage)
    {
        mStagesList.add(stage);
    }

    PipelineStageRef PipelineStageManager::findStage(uint32 stageID) const
    {
        for (auto stage = mStagesList.begin(); stage != nullptr; stage = mStagesList.next())
        {
            if ((*stage)->getStageID() == stageID)
                return *stage;
        }

        return PipelineStageRef();
    }

    PipelineStageRef PipelineStageManager::findStage(const char *name) const
    {
        for (auto stage = mStagesList.begin(); stage != nullptr; stage = mStagesList.next())
        {
            if ((*stage)->getStageName() == name)
                return *stage;
        }

        return PipelineStageRef();
    }

} // namespace Berserk