/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERMETADATA_H
#define BERSERK_RHISHADERMETADATA_H

#include <RHI/RHIResource.h>
#include <RHI/RHIShaderAttribute.h>
#include <RHI/RHIShaderParam.h>
#include <RHI/RHIShaderBlockMember.h>
#include <RHI/RHIShaderUniformBlock.h>

namespace Berserk {

    class RHIShaderMetaData : public RHIResource {
    public:
        ~RHIShaderMetaData() override = default;

        static const uint32 MAX_UNIFORM_NAME_LENGTH = 512;
        static const uint32 MAX_UNIFORMS_COUNT = 512;

        /** @return Shader param found via raw search (possibly null) */
        TRef<const ShaderParam> getParam(const char* name) const;

        /** @return Shader uniform block found via raw search (possibly null) */
        TRef<const ShaderUniformBlock> getUniformBlock(const char* name) const;

        /** @return Shader param found via map look-up (possibly null) */
        TRef<const ShaderParam> findParam(const CString& name) const;

        /** @return Shader uniform block found via map look-up (possibly null) */
        TRef<const ShaderUniformBlock> findUniformBlock(const CString& name) const;

        /** @return Program variables */
        const TArray<ShaderParam> &getParams() const { return mParams; }

        /** @return Program uniform blocks info */
        const TArray<ShaderUniformBlock> &getUniformBlocks() const { return mUniformBlocks; }

        /** @return Input attributes of the vertex shader (as main entry for graphics shader)*/
        const TArray<ShaderAttribute> &getVertexShaderAttributes() const { return mVertexShaderAttributes; }

        /** @return Input attributes of the fragment shader */
        const TArray<ShaderAttribute> &getFragmentShaderAttributes() const { return mFragmentShaderAttributes; }

        /** @return True if meta has uniform block with specified name */
        bool hasUniformBlock(const CString& name) const { return mUniformBlocksIdx.contains(name); }

        /** Serialization for shader caching */
        virtual void serialize(Archive& archive) const = 0;

        /** Deserialization for shader caching */
        virtual void deserialize(Archive& archive) = 0;

        /** Serialization for shader caching (syntax sugar) */
        friend Archive& operator<<(Archive& archive, const RHIShaderMetaData& meta) {
            meta.serialize(archive);
            return archive;
        }

        /** Deserialization for shader caching (syntax sugar) */
        friend Archive& operator>>(Archive& archive, RHIShaderMetaData& meta) {
            meta.deserialize(archive);
            return archive;
        }

        /** Output debug info */
        void showDebugInfo() const;

    protected:
        TMap<CString,uint32> mParamsIdx;
        TMap<CString,uint32> mUniformBlocksIdx;
        TArray<ShaderParam> mParams;
        TArray<ShaderUniformBlock> mUniformBlocks;
        TArray<ShaderAttribute> mVertexShaderAttributes;
        TArray<ShaderAttribute> mFragmentShaderAttributes;
    };

}

#endif //BERSERK_RHISHADERMETADATA_H