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
#include <ShaderProgram.h>
#include <ShaderBindings.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader class
         *
         * Represents an immutable instance of the shader program.
         *
         * Instances of this class share immutable compiled RHI sources, what
         * allows to reuse RHI resources and speed-up shaders loading.
         *
         * Instances of this class provided by specific ShaderFactory classes, which are
         * capable of creating and loading shaders of specific type.
         */
        class Shader {
        public:

            /** Shader from name, program and declaration references */
            Shader(CString name, TPtrShared<ShaderProgram> program, TPtrShared<VertexDeclaration> declaration);

            /** Virtual, since we track shaders by pointers  */
            virtual ~Shader() = default;

            /** @return Unique shader name */
            const CString &getName() const { return mName; }

            /** @return Program */
            const TPtrShared<ShaderProgram> &getProgram() const { return mProgram; }

            /** @return Declaration */
            const TPtrShared<VertexDeclaration> &getDeclaration() const { return mDeclaration; }

            /** @return Shader RHI resource */
            const TPtrShared<RHIShader> &getShaderRHI() const { return mProgram->getShader(); }

            /** @return Shader meta data RHI resource */
            const TPtrShared<RHIShaderMetaData> &getShaderMetaRHI() const { return mProgram->getMetaData(); }

            /** @return Vertex declaration RHI resource */
            const TPtrShared<RHIVertexDeclaration> &getDeclarationRHI() const { return mDeclaration->getRHI(); }

            /** @return Allocates uniform bindings for this shader */
            TPtrShared<class ShaderBindings> allocateBindings() const;

        protected:
            CString mName;
            TPtrShared<ShaderProgram> mProgram;
            TPtrShared<VertexDeclaration> mDeclaration;
        };

    }
}

#endif //BERSERK_SHADER_H