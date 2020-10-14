/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIPROGRAM_H
#define BERSERK_RHIPROGRAM_H

#include <BerserkCore/RHI/RHIDefinitions.hpp>
#include <BerserkCore/RHI/RHIResource.hpp>
#include <BerserkCore/Containers/TArrayFixed.hpp>
#include <BerserkCore/String/String.hpp>

namespace Berserk {

    using RHIProgramStages = TArrayFixed<RHIProgramStage,RHI::MAX_PROGRAM_STAGES>;

    class RHIProgram: public RHIResource {
    public:
        ~RHIProgram() override = default;
        const RHIProgramStages& GetStages() const { return mStages; }
        const String& GetName() const { return mName; }
        bool IsSourcesCached() const { return mIsSourcesCached; }
        EShaderLang GetShaderLanguage() const { return mLanguage; }

    private:
        RHIProgramStages mStages;
        String mName;
        bool mIsSourcesCached;
        EShaderLang mLanguage;
    };

}

#endif //BERSERK_RHIPROGRAM_H