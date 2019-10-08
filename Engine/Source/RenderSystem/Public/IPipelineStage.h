//
// Created by Egor Orachyov on 2019-08-14.
//

#ifndef BERSERK_IPIPELINESTAGE_H
#define BERSERK_IPIPELINESTAGE_H

#include <HAL/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>
#include <Pointer/TSharedPtr.h>
#include <PipelinePassContext.h>

namespace Berserk
{

    /**
     * Single render system pipeline stage, which operates on some shader
     * pipeline input data and makes single piece of work.
     *
     * @note All the stages created and manager by stage manager.
     *       Each stage could be used by pipeline scheduler, which
     *       defines order of the stage execution.
     *
     * @note Each pipeline mast operate on only one effect/work.
     *       Common stages: shadow maps generation, generating queues for elements,
     *       culling, post effects, lightning, etc.
     *
     * @note All the render scenes with defined render view to single
     *       camera will be passed to the pipeline scheduler by one will be passed
     *       to execute and generate final output image.
     *
     * @note Single-thread
     */
	class RENDER_API IPipelineStage
	{
	public:

		virtual ~IPipelineStage() = default;

		/**
		 * Actually executes this state.
		 * Called each frame for each render view.
		 */
		virtual void execute(PipelinePassContext& context) = 0;

		/**
		 * Reloads internal state data.
		 * Called if some shared data was changed.
		 */
		virtual void reload() = 0;

		/** @return Stage ID */
		uint32 getStageID() const { return mStageID; }

        /** @return Stage name */
		const String& getStageName() const { return mStageName; }

    private:

	    /** Unique stage id for look-up */
	    uint32 mStageID = getNextStageID();

	    /** Pipeline stage name [for search/debug] */
	    String mStageName;

    protected:

	    /** @return Unique stage id for look-up */
	    static uint32 getNextStageID()
	    {
	        static uint32 id = 0;
            return ++id;
	    }

	};

	typedef TSharedPtr<IPipelineStage> PipelineStageRef;

} // namespace Berserk

#endif //BERSERK_IPIPELINESTAGE_H