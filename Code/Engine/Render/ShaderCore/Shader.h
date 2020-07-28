/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADER_H
#define BERSERK_SHADER_H

#include <Paths.h>
#include <ShaderCore/ShaderProgram.h>
#include <ShaderCore/ShaderUniformBindings.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader class
         *
         * Represents an immutable instance of the shader program and complete graphics pipeline,
         * i.e. fully configured RHI graphics state, required for rendering geometry on GPU.
         *
         * Instances of this class share immutable compiled RHI sources, what
         * allows to reuse RHI resources and speed-up shaders loading.
         *
         * Instances of this class provided by specific ShaderFactory classes, which are
         * capable of creating and loading shaders of specific type.
         */
        class Shader {
        public:

            Shader(CString name, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration, RHIGraphicsPipelineState pipelineState);

            /** Virtual, since we track shader by pointers  */
            virtual ~Shader() = default;

            /** Use this shader program with uniform data in specified draw list */
            void use(RHIDrawList& drawList);

            const CString &getName() const { return mName; }

            const TimeValue &getTimeLastUsed() const { return mTimeLastUsed; }

            const TPtrShared<ShaderProgram> &getProgram() const { return mProgram; }

            const TPtrShared<VertexDeclaration> &getDeclaration() const { return mDeclaration; }

            const RHIGraphicsPipelineState &getPipelineState() const { return mPipelineState; }

        protected:
            CString mName;
            TimeValue mTimeLastUsed;
            TPtrShared<ShaderProgram> mProgram;
            TPtrShared<VertexDeclaration> mDeclaration;
            RHIGraphicsPipelineState mPipelineState;
        };

    }
}

#endif //BERSERK_SHADER_H