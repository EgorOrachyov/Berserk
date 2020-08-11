/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERPARAM_H
#define BERSERK_RHISHADERPARAM_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /** Bindable params (textures, samplers) */
    class ShaderParam {
    public:

        ShaderParam() = default;
        ShaderParam(CString name, uint32 location, EShaderData dataType, TEnumMask<EShaderType> stageFlags) {
            mName = std::move(name);
            mLocation = location;
            mData = dataType;
            mStageFlags = stageFlags;
        }

        /** @return Name of the param in the shader */
        const CString& getName() const { return mName; }

        /** @return Native location (binding) for shader access */
        uint32 getLocation() const { return mLocation; }

        /** @return Type of the param */
        EShaderData getDataType() const { return mData; }

        /** @return Stage usage flags of the param */
        TEnumMask<EShaderType> getStageFlags() const { return mStageFlags; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderParam& param) {
            archive << param.mName;
            archive << param.mLocation;
            archive << (uint32)param.mData;
            archive << param.mStageFlags;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderParam& param) {
            archive >> param.mName;
            archive >> param.mLocation;
            archive >> (uint32&)param.mData;
            archive >> param.mStageFlags;
            return archive;
        }

    private:
        CString mName;
        uint32 mLocation;
        EShaderData mData;
        TEnumMask<EShaderType> mStageFlags;
    };

}

#endif //BERSERK_RHISHADERPARAM_H