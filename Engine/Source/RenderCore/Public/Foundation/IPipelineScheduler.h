//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_IPIPELINESCHEDULER_H
#define BERSERK_IPIPELINESCHEDULER_H

#include <Foundation/IPipelineStage.h>

namespace Berserk::Render
{

    /**
     * One render system pipeline stage, which handles one
     * image effect or rendering process, such as GBuffer generation,
     * ToneMapping, etc.
     */
    class IPipelineScheduler : public RenderBase
    {
    public:

        virtual ~IPipelineScheduler() = default;

        /** Adds new stage in the end of the scheduler */
        virtual void addStage(IPipelineStage* stage) = 0;

        /** Called each render frame: executes all the render system pipeline stages */
        virtual void execute(RenderPassInfo& passInfo) = 0;

    };

} // namespace Berserk::Render

#endif //BERSERK_IPIPELINESCHEDULER_H