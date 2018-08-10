//
// Created by Egor Orachyov on 07.08.2018.
//

#ifndef BERSERKENGINE_PIPELINESTAGE_H
#define BERSERKENGINE_PIPELINESTAGE_H

namespace Berserk
{

    class PipelineStage
    {
    public:

        virtual ~PipelineStage() = default;
        virtual void init() = 0;
        virtual void destroy() = 0;
        virtual void execute() = 0;

    };

} // namespace Berserk

#endif //BERSERKENGINE_PIPELINESTAGE_H