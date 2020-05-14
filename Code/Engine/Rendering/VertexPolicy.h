/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXPOLICY_H
#define BERSERK_VERTEXPOLICY_H

#include <Containers/TArray.h>
#include <Typedefs.h>
#include <TEnumMask.h>
#include <TPtrShared.h>
#include <Containers/TArrayStatic.h>
#include <RHI/RHIResources.h>

namespace Berserk {
    namespace Rendering {

        enum class EVertex {
            Position = 0,
            Normal = 1,
            Tangent = 2,
            Bitangent = 3,
            TexCoords = 4,
            Color = 5
        };

        /** @brief Describes an vertex shader input */
        class VertexInput {
        public:

            explicit VertexInput(const TEnumMask <EVertex> &mask) : mVertexMask(mask) {}
            VertexInput() = default;
            ~VertexInput() = default;

            VertexInput &operator=(const VertexInput &other) = default;
            VertexInput &operator=(VertexInput &&other) = default;

            bool hasElement(EVertex element) const { return mVertexMask.getFlag(element); }
            bool hasPosition() const { return mVertexMask.getFlag(EVertex::Position); }
            bool hasNormal() const { return mVertexMask.getFlag(EVertex::Normal); }
            bool hasTangent() const { return mVertexMask.getFlag(EVertex::Tangent); }
            bool hasBitangent() const { return mVertexMask.getFlag(EVertex::Bitangent); }
            bool hasTexCoords() const { return mVertexMask.getFlag(EVertex::TexCoords); }
            bool hasColor() const { return mVertexMask.getFlag(EVertex::Color); }

            TEnumMask <EVertex> getMask() const { return mVertexMask; }

            /** @return Vertex element size in bytes */
            static uint32 getVertexElementSize(EVertex element);
            /** @return Vertex element name */
            static const char *getVertexElementName(EVertex element);
            /** @return Vertex element to type */
            static EVertexElementType getVertexElementType(EVertex element);

        private:
            TEnumMask <EVertex> mVertexMask;
        };

        /** @brief Describes layout of the elements in the memory */
        class VertexPolicy {
        public:

            /** Max input parameters to the vertex shader */
            static const uint32 MAX_VERTEX_SHADER_INPUTS = 8;

            /**
             * @brief Create vertex policy from ordered sequence of the attributes
             *
             * @example Consider the following sequence of the attributes: {Position,Normal,TexCoords}
             *          Then the result layout of the attributes will be creates as follows:
             *          - {Float3,location 0,offset 0,stride} for Position
             *          - {Float3,location 1,offset sizeof(Float3),stride} for Normal
             *          - {Float2,location 2,offset sizeof(Float3)*2,stride} for TexCoords
             *          - stride = sizeof(Float3) + sizeof(Float3) + sizeof(Float2)
             */
            VertexPolicy(const std::initializer_list<EVertex> &elements);
            VertexPolicy();

            void addAttribute(EVertex element, uint32 offset, uint32 location);

            uint32 getOffset(EVertex element) const;
            uint32 getStride() const { return mStride; }
            uint32 getAttributesCount() const { return mElementsList.size(); }

            void getVertexDeclarationDesc(RHIVertexDeclarationDesc &desc, uint32 buffer = 0) const;

            const VertexInput &getInput() const { return mVertexInput; }
            const TArrayStatic<EVertex> getElementsList() const { return mElementsList; }
            const TArrayStatic<RHIVertexElement> getElementsDescs() const { return mElementsDescs; }

        protected:
            uint32 mStride = 0;
            VertexInput mVertexInput;
            TArrayStatic<EVertex> mElementsList;
            TArrayStatic<RHIVertexElement, MAX_VERTEX_SHADER_INPUTS> mElementsDescs;
        };

        /** Default vertex policies */
        enum class EVertexPolicy {
            Position,
            PositionNormal,
            PositionNormalTexCoords,
            PositionNormalTangentTexCoords,
            PositionNormalTangentBitangentTexCoords
        };

        /**
         * @brief Default vertex policies factory
         *
         * Engine vertex policy factory for default attributes layouts.
         * Created by RenderModule.
         */
        class VertexPolicyFactory {
        public:

            VertexPolicyFactory();
            ~VertexPolicyFactory() = default;

            /**
             * @brief Default policy factory
             * @param policy Policy to get
             * @return Pointer to cached policy
             */
            const TPtrShared <VertexPolicy> &getPolicy(EVertexPolicy policy);

            /**
             * @brief Default RHI vertex declaration reference
             * @param policy The predefined source police
             * @return Reference to the RHI vertex declaration object
             */
            const TPtrShared <RHIVertexDeclaration> &getDeclaration(EVertexPolicy policy);

        private:
            TPtrShared<VertexPolicy> mPosPolicy;
            TPtrShared<VertexPolicy> mPosNormPolicy;
            TPtrShared<VertexPolicy> mPosNormTexCoordsPolicy;
            TPtrShared<VertexPolicy> mPosNormTangentTexCoordsPolicy;
            TPtrShared<VertexPolicy> mPosNormTangentBitangentTexCoordsPolicy;

            TPtrShared<RHIVertexDeclaration> mPosDeclaration;
            TPtrShared<RHIVertexDeclaration> mPosNormDeclaration;
            TPtrShared<RHIVertexDeclaration> mPosNormTexCoordsDeclaration;
            TPtrShared<RHIVertexDeclaration> mPosNormTangentTexCoordsDeclaration;
            TPtrShared<RHIVertexDeclaration> mPosNormTangentBitangentTexCoordsDeclaration;
        };

    }
}

#endif //BERSERK_VERTEXPOLICY_H