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

        enum class EVertexAttribute {
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

            explicit VertexInput(const TEnumMask <EVertexAttribute> &mask) : mVertexMask(mask) {}
            VertexInput() = default;
            VertexInput(const VertexInput& input) = default;
            VertexInput(VertexInput&& input) = default;

            VertexInput &operator=(const VertexInput &other) = default;
            VertexInput &operator=(VertexInput &&other) = default;

            bool hasElement(EVertexAttribute element) const { return mVertexMask.getFlag(element); }
            bool hasPosition() const { return mVertexMask.getFlag(EVertexAttribute::Position); }
            bool hasNormal() const { return mVertexMask.getFlag(EVertexAttribute::Normal); }
            bool hasTangent() const { return mVertexMask.getFlag(EVertexAttribute::Tangent); }
            bool hasBitangent() const { return mVertexMask.getFlag(EVertexAttribute::Bitangent); }
            bool hasTexCoords() const { return mVertexMask.getFlag(EVertexAttribute::TexCoords); }
            bool hasColor() const { return mVertexMask.getFlag(EVertexAttribute::Color); }

            TEnumMask <EVertexAttribute> getMask() const { return mVertexMask; }

            /** @return Vertex element size in bytes */
            static uint32 getVertexElementSize(EVertexAttribute element);
            /** @return Vertex element name */
            static const char *getVertexElementName(EVertexAttribute element);
            /** @return Vertex element to type */
            static EVertexElementType getVertexElementType(EVertexAttribute element);

        private:
            TEnumMask <EVertexAttribute> mVertexMask;
        };

        /** @brief Describes layout of the elements in the memory */
        class VertexPolicy {
        public:
            /** Max input parameters to the vertex shader */
            static const uint32 MAX_VERTEX_SHADER_INPUTS = 8;

            VertexPolicy() = default;
            VertexPolicy(const VertexPolicy& policy) = default;
            VertexPolicy(VertexPolicy&& policy) = default;

            uint32 getBufferIndex(EVertexAttribute element) const;
            uint32 getOffset(EVertexAttribute element) const;
            uint32 getStride(EVertexAttribute element) const;
            uint32 getAttributesCount() const { return mElementsDescs.size(); }

            const VertexInput &getInput() const { return mVertexInput; }
            const TArray<RHIVertexElement> &getElementsDescs() const { return mElementsDescs; }
            const RHIVertexDeclarationDesc &getElementsDescsRHI() const { return mElementsDescs; }

        protected:
            friend class VertexPolicyBuilder;

            uint32 mHash = 0;
            VertexInput mVertexInput;
            TArrayStatic<uint32> mNamedAttributes;
            TArray<RHIVertexElement> mElementsDescs;
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