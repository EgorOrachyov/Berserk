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
#include <RenderResources/GraphicsPipeline.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader class
         *
         * Represents an instance of the shader program and complete graphics pipeline,
         * with all the rasterization settings and uniform data, required for the rendering.
         *
         * Instances of this class share immutable compiled RHI sources, what
         * allows to reuse RHI resources and speed-up shaders loading.
         *
         * This class must be extended in order to create an own type of the shader program
         * with types safe and documented access to shader uniform data.
         */
        class Shader {
        public:

            Shader() = default;
            virtual ~Shader() = default;

            /** @return True if shader can be used */
            bool isUsable() const;

            /** Use this shader program with uniform data in specified draw list */
            void use(RHIDrawList& drawList);

        protected:

            void setName(CString name);
            bool initializeProgram(const CString& pathToShader, EPathType pathType);
            bool initializeUniformData(class ContextUniformData& context);

            CString mName;
            TimeValue mTimeLastUsed;
            TPtrShared<ShaderProgram> mProgram;
            TPtrShared<ShaderUniformBindings> mUniformData;
            TPtrShared<GraphicsPipeline> mPipeline;
            TPtrShared<VertexDeclaration> mDeclaration;

        };

    }
}

#endif //BERSERK_SHADER_H