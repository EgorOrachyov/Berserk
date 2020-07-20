/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXPOLICYBUILDER_H
#define BERSERK_VERTEXPOLICYBUILDER_H

#include <Rendering/VertexPolicy.h>

namespace Berserk {
    namespace Rendering {

        /** Vertex policy builder for generic and named policy attributes */
        class VertexPolicyBuilder final {
        public:

            VertexPolicyBuilder() {
                mNamedAttributes.resize(VertexPolicy::MAX_VERTEX_SHADER_INPUTS);
            }

            VertexPolicyBuilder& ensureElements(uint32 count) {
                mElements.ensureToAdd(count);

                return *this;
            }

            VertexPolicyBuilder& addBuffer(const TArrayStatic<EVertexElementType> &attributes) {
                mElements.ensureToAdd(attributes.size());
                uint32 stride = 0;

                for (auto attribute: attributes) {
                    stride += RHIDefinitionsUtil::getVertexElementSize(attribute);
                }

                addBuffer(stride);

                for (auto attribute: attributes) {
                    addAttribute(attribute);
                }

                return *this;
            }

            VertexPolicyBuilder& addBufferInstanced(const TArrayStatic<EVertexElementType> &attributes) {
                mElements.ensureToAdd(attributes.size());
                uint32 stride = 0;

                for (auto attribute: attributes) {
                    stride += RHIDefinitionsUtil::getVertexElementSize(attribute);
                }

                addBufferInstanced(stride);

                for (auto attribute: attributes) {
                    addAttribute(attribute);
                }

                return *this;
            }

            VertexPolicyBuilder& addNamedBuffer(const TArrayStatic <EVertexAttribute> &attributes) {
                mElements.ensureToAdd(attributes.size());
                uint32 stride = 0;

                for (auto attribute: attributes) {
                    stride += VertexInput::getVertexElementSize(attribute);
                }

                addBuffer(stride);

                for (auto attribute: attributes) {
                    addNamedAttribute(attribute);
                }

                return *this;
            }

            VertexPolicyBuilder& addBuffer(uint32 stride) {
                mBufferIndex = mNextBuffer; mNextBuffer += 1;
                mBufferStride = stride;
                mBufferCurrentOffset = 0;
                mBufferIterating = EVertexIterating::PerVertex;

                return *this;
            }

            VertexPolicyBuilder& addBufferInstanced(uint32 stride) {
                mBufferIndex = mNextBuffer; mNextBuffer += 1;
                mBufferStride = stride;
                mBufferCurrentOffset = 0;
                mBufferIterating = EVertexIterating::PerInstance;

                return *this;
            }

            VertexPolicyBuilder& addAttribute(EVertexElementType type) {
                addAttribute(mBufferCurrentLocation, mBufferCurrentOffset, type);
                mBufferCurrentLocation += 1;
                mBufferCurrentOffset += RHIDefinitionsUtil::getVertexElementSize(type);

                return *this;
            }

            VertexPolicyBuilder& addNamedAttribute(EVertexAttribute type) {
                addNamedAttribute(mBufferCurrentLocation, mBufferCurrentOffset, type);
                mBufferCurrentLocation += 1;
                mBufferCurrentOffset += VertexInput::getVertexElementSize(type);

                return *this;
            }

            VertexPolicyBuilder& addAttribute(uint32 location, EVertexElementType type) {
                addAttribute(location, mBufferCurrentOffset, type);
                mBufferCurrentOffset += RHIDefinitionsUtil::getVertexElementSize(type);

                return *this;
            }

            VertexPolicyBuilder& addNamedAttribute(uint32 location, EVertexAttribute type) {
                addNamedAttribute(location, mBufferCurrentOffset, type);
                mBufferCurrentOffset += VertexInput::getVertexElementSize(type);

                return *this;
            }

            VertexPolicyBuilder& addAttribute(uint32 location, uint32 offset, EVertexElementType type) {
                RHIVertexElement element{};
                element.buffer = mBufferIndex;
                element.iterating = mBufferIterating;
                element.location = location;
                element.offset = offset;
                element.stride = mBufferStride;
                element.type = type;

                mElements.move(element);

                return *this;
            }

            VertexPolicyBuilder& addNamedAttribute(uint32 location, uint32 offset, EVertexAttribute type) {
                if (mVertexInput.hasElement(type))
                    return *this;

                RHIVertexElement element{};
                element.buffer = mBufferIndex;
                element.iterating = mBufferIterating;
                element.location = location;
                element.offset = offset;
                element.stride = mBufferStride;
                element.type = VertexInput::getVertexElementType(type);

                auto index = mElements.size();
                mNamedAttributes[(uint32)type] = index;
                mVertexInput = VertexInput(mVertexInput.getMask().setFlag(type, true));
                mElements.move(element);

                return *this;
            }

            VertexPolicy build() {
                VertexPolicy policy;
                policy.mVertexInput = mVertexInput;
                policy.mNamedAttributes = mNamedAttributes;
                policy.mElementsDescs = std::move(mElements);

                Crc32::Builder builder;

                builder.hashRaw(&policy.mVertexInput, sizeof(policy.mVertexInput));
                policy.mNamedAttributes.forEach([&](const uint32& v){ builder.hashRaw(&v, sizeof(uint32)); });
                policy.mElementsDescs.forEach([&](const RHIVertexElement& v){ v.buildHash(builder); });
                policy.mHash = builder.getHash();

                return policy;
            }

            TPtrShared<VertexPolicy> buildShared() {
                auto policy = TPtrShared<VertexPolicy>::make();
                policy->mVertexInput = mVertexInput;
                policy->mNamedAttributes = mNamedAttributes;
                policy->mElementsDescs = std::move(mElements);

                Crc32::Builder builder;

                policy->mHash =
                 builder
                  .hashT(policy->mVertexInput)
                  .hashT(policy->mNamedAttributes.hash<THashRaw<uint32>>())
                  .hashT(policy->mElementsDescs.hash())
                  .build();

                return policy;
            }

        private:
            uint32 mNextBuffer = 0;
            uint32 mBufferIndex = 0;
            uint32 mBufferStride = 0;
            uint32 mBufferCurrentOffset = 0;
            uint32 mBufferCurrentLocation = 0;
            EVertexIterating mBufferIterating = EVertexIterating::PerVertex;

            VertexInput mVertexInput;
            TArray<RHIVertexElement> mElements;
            TArrayStatic<uint32, VertexPolicy::MAX_VERTEX_SHADER_INPUTS> mNamedAttributes;
        };

    }
}

#endif //BERSERK_VERTEXPOLICYBUILDER_H