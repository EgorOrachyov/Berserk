//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_IPIPELINESCHEDULER_H
#define BERSERK_IPIPELINESCHEDULER_H

#include <Foundation/IPipelineStage.h>

namespace Berserk::Render
{

    /**
     * Allows to control flow and execute one frame pipeline
     * stages, registered by render system
     */
    class ENGINE_API IPipelineScheduler : public RenderBase
    {
    public:

        /** Nothing */
        virtual ~IPipelineScheduler() = default;

        /** Adds new stage in the end of the scheduler */
        virtual void addStage(IPipelineStage* stage) = 0;

        /** Called each render frame: executes all the render system pipeline stages */
        virtual void execute(RenderPassInfo& passInfo) = 0;

        /** @return Buffer with all the scheduler stages */
        virtual IPipelineStage** getStages() = 0;

        /** @return Number of added stages */
        virtual uint32 stagesCount() = 0;
    };

} // namespace Berserk::Render

#endif //BERSERK_IPIPELINESCHEDULER_H