//
// Created by Egor Orachyov on 2019-08-22.
//

#ifndef BERSERK_PIPELINESTAGEMANAGER_H
#define BERSERK_PIPELINESTAGEMANAGER_H

#include <IPipelineStage.h>
#include <Containers/TArray.h>

namespace Berserk
{

    /**
     * Manages all the loaded pipeline stages for reuse.
     *
     * @note All the pipeline stages, supported by current hardware
     *       loaded by render system in time of the initialization.
     *
     * @note If scheduler requires pipeline stage, then it will be loaded
     *       or found via name by manager.
     *
     * @note Single-thread
     */
    class RENDER_API PipelineStageManager
    {
    public:

        /**
         * Initialize empty pipeline stage manager
         * @param stages Default stages count to preallocate
         * @param allocator Manager to allocate internal list with stages
         */
        explicit PipelineStageManager(uint32 stages = DEFAULT_STAGES_COUNT, IAllocator& allocator = Allocator::get());

        ~PipelineStageManager() = default;

        /** Add fully loaded stage to the manager */
        void addStage(const PipelineStageRef& stage);

        /** @return Found stage via unique ID */
        PipelineStageRef findStage(uint32 stageID) const;

        /** @return Found stage via unique string name */
        PipelineStageRef findStage(const char* name) const;

    protected:

        static const uint32 DEFAULT_STAGES_COUNT = 8;
        typedef TArray<PipelineStageRef> StagesList;

        /** List, contains all the loaded stages */
        StagesList mStagesList;

    };

} // namespace Berserk

#endif //BERSERK_PIPELINESTAGEMANAGER_H