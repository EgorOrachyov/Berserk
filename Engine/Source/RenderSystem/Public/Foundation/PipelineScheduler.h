//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_PIPELINESCHEDULER_H
#define BERSERK_PIPELINESCHEDULER_H

#include <Containers/ArrayList.h>
#include <Foundation/IObjectMacros.h>
#include <Foundation/IPipelineScheduler.h>

namespace Berserk::Render
{

    /**
     * Allows to control flow and execute one frame pipeline
     * stages, registered by render system
     */
    class PipelineScheduler : public IPipelineScheduler
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(PipelineScheduler);

        /** Create via pointer to allocator for internal tasks */
        explicit PipelineScheduler(IAllocator* allocator);

        /** @copydoc IPipelineScheduler::~PipelineScheduler() */
        ~PipelineScheduler() override = default;

        /** @copydoc IPipelineScheduler::addStage() */
        void addStage(IPipelineStage* stage) override;

        /** @copydoc IPipelineScheduler::execute() */
        void execute(RenderPassInfo& passInfo) override;

    protected:

        static const uint32 INITIAL_STAGES_COUNT = 16;

        /** List all registered stages to be executed */
        ArrayList<IPipelineStage*> mStages;

    };

} // namespace Berserk::Render

#endif //BERSERK_PIPELINESCHEDULER_H