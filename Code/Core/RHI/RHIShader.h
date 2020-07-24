/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADER_H
#define BERSERK_RHISHADER_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIShader : public RHIResource {
    public:
        ~RHIShader() override = default;

        /** @return Sub-stages of this gpu shader program */
        const TArrayStatic<EShaderType>& getShaderStages() const { return mShaderStages; }

        /** @return True if shader could be cached to the file on disk */
        virtual bool supportsSerialization() const = 0;

        /** @return True if successfully cache shader binary data in buffer */
        virtual bool serialize(BinaryData &buffer) const = 0;

    protected:
        /** Sub-stages of this gpu shader program */
        TArrayStatic<EShaderType> mShaderStages;
    };

}

#endif //BERSERK_RHISHADER_H
