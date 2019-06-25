//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_IPIPELINESTAGE_H
#define BERSERK_IPIPELINESTAGE_H

#include <Strings/Old/String.h>
#include <Foundation/RenderBase.h>
#include <Foundation/RenderPassInfo.h>

namespace Berserk::Render
{

    /**
     * One render system pipeline stage, which handles one
     * image effect or rendering process, such as GBuffer generation,
     * ToneMapping, etc.
     */
    class ENGINE_API IPipelineStage : public RenderBase
    {
    public:

        GENARATE_NEW_DELETE(IPipelineStage);

        /** Initialize via name and pointer to general purpose allocator */
        explicit IPipelineStage(const char* name, IAllocator* allocator)
                : mStageName(name),
                  mGenAllocator(allocator)

        {
            FAIL(allocator, "Null pointer allocator");
        }

        virtual ~IPipelineStage() = default;

        /** Called when render system setting updated */
        virtual void reload() = 0;

        /** Called each render frame */
        virtual void execute(RenderPassInfo& passInfo) = 0;

        /** @return Name of this stage (for profiling) */
        virtual const char* getName() { return mStageName.get(); }

    protected:

        /** Name of this stage */
        CString mStageName;

        /** For internal usage */
        class IAllocator* mGenAllocator;
    };

} // namespace Berserk::Render

#endif //BERSERK_IPIPELINESTAGE_H