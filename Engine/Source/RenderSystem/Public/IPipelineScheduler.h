//
// Created by Egor Orachyov on 2019-08-14.
//

#ifndef BERSERK_IPIPELINESCHEDULER_H
#define BERSERK_IPIPELINESCHEDULER_H

#include <Strings/String.h>
#include <PipelinePassContext.h>

namespace Berserk
{

    /**
     * Scheduler defines single way of pipeline stages execution.
     * Basically includes all the stages, needed for rendering
     * each scene on the screen.
     *
     * @note Single-thread
     */
    class RENDER_API IPipelineScheduler
    {
    public:

        /**
         * Executes all the stages of this scheduler for single camera view.
         * Calls one by one stages, and pass result/param through function params.
         *
         * @note Possibly too expensive call.
         *       Involves highly loaded fragment shader computations [for GPU RHI]
         */
        virtual void execute(PipelinePassContext& context) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IPIPELINESCHEDULER_H