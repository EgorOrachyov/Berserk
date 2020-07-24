/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <VertexPolicy.h>
#include <VertexPolicyBuilder.h>
#include <RHI/RHIDevice.h>
#include <LogMacro.h>

namespace Berserk {
    namespace Rendering {

        uint32 VertexInput::getVertexElementSize(EVertexAttribute element) {
            switch (element) {
                case EVertexAttribute::Position:
                    return sizeof(float) * 3;
                case EVertexAttribute::Normal:
                    return sizeof(float) * 3;
                case EVertexAttribute::Tangent:
                    return sizeof(float) * 3;
                case EVertexAttribute::Bitangent:
                    return sizeof(float) * 3;
                case EVertexAttribute::TexCoords:
                    return sizeof(float) * 2;
                case EVertexAttribute::Color:
                    return sizeof(float) * 3;
                default:
                    return 0;
            }
        }

        const char *VertexInput::getVertexElementName(EVertexAttribute element) {
            switch (element) {
                case EVertexAttribute::Position:
                    return "Position";
                case EVertexAttribute::Normal:
                    return "Normal";
                case EVertexAttribute::Tangent:
                    return "Tangent";
                case EVertexAttribute::Bitangent:
                    return "Bitangent";
                case EVertexAttribute::TexCoords:
                    return "TexCoords";
                case EVertexAttribute::Color:
                    return "Color";
                default:
                    return "Undefined";
            }
        }

        EVertexElementType VertexInput::getVertexElementType(EVertexAttribute element) {
            switch (element) {
                case EVertexAttribute::Position:
                    return EVertexElementType::Float3;
                case EVertexAttribute::Normal:
                    return EVertexElementType::Float3;
                case EVertexAttribute::Tangent:
                    return EVertexElementType::Float3;
                case EVertexAttribute::Bitangent:
                    return EVertexElementType::Float3;
                case EVertexAttribute::TexCoords:
                    return EVertexElementType::Float2;
                case EVertexAttribute::Color:
                    return EVertexElementType::Float3;
                default:
                    return EVertexElementType::Unknown;
            }
        }

        uint32 VertexPolicy::getBufferIndex(Berserk::Rendering::EVertexAttribute element) const {
            auto &input = getInput();

            if (!input.hasElement(element))
                return -1;

            auto index = mNamedAttributes[(uint32)element];
            return mElementsDescs[index].buffer;
        }

        uint32 VertexPolicy::getOffset(EVertexAttribute element) const {
            auto &input = getInput();

            if (!input.hasElement(element))
                return 0;

            auto index = mNamedAttributes[(uint32)element];
            return mElementsDescs[index].offset;
        }

        uint32 VertexPolicy::getStride(EVertexAttribute element) const {
            auto &input = getInput();

            if (!input.hasElement(element))
                return 0;

            auto index = mNamedAttributes[(uint32)element];
            return mElementsDescs[index].stride;
        }

        VertexPolicyFactory::VertexPolicyFactory() {
            using EVX = EVertexAttribute;
            using IL = std::initializer_list<EVertexAttribute>;

            {
                VertexPolicyBuilder builder;
                builder.addNamedBuffer(IL{EVX::Position});
                mPosPolicy = builder.buildShared();
            }

            {
                VertexPolicyBuilder builder;
                builder.addNamedBuffer(IL{EVX::Position, EVX::Normal});
                mPosNormPolicy = builder.buildShared();
            }

            {
                VertexPolicyBuilder builder;
                builder.addNamedBuffer(IL{EVX::Position, EVX::Normal, EVX::TexCoords});
                mPosNormTexCoordsPolicy = builder.buildShared();
            }

            {
                VertexPolicyBuilder builder;
                builder.addNamedBuffer(IL{EVX::Position, EVX::Normal, EVX::Tangent, EVX::TexCoords});
                mPosNormTangentTexCoordsPolicy = builder.buildShared();
            }

            {
                VertexPolicyBuilder builder;
                builder.addNamedBuffer(IL{EVX::Position, EVX::Normal, EVX::Tangent, EVX::Bitangent, EVX::TexCoords});
                mPosNormTangentBitangentTexCoordsPolicy = builder.buildShared();
            }

            auto &device = RHIDevice::getSingleton();
            const RHIVertexDeclarationDesc *declarationDesc;

            declarationDesc = &mPosPolicy->getElementsDescsRHI();
            mPosDeclaration = device.createVertexDeclaration(*declarationDesc);

            declarationDesc = &mPosNormPolicy->getElementsDescsRHI();
            mPosNormDeclaration = device.createVertexDeclaration(*declarationDesc);

            declarationDesc = &mPosNormTexCoordsPolicy->getElementsDescsRHI();
            mPosNormTexCoordsDeclaration = device.createVertexDeclaration(*declarationDesc);

            declarationDesc = &mPosNormTangentTexCoordsPolicy->getElementsDescsRHI();
            mPosNormTangentTexCoordsDeclaration = device.createVertexDeclaration(*declarationDesc);

            declarationDesc = &mPosNormTangentBitangentTexCoordsPolicy->getElementsDescsRHI();
            mPosNormTangentBitangentTexCoordsDeclaration = device.createVertexDeclaration(*declarationDesc);

            BERSERK_LOG_INFO("Initialize VertexPolicyFactory");
        }

        const TPtrShared<VertexPolicy> &VertexPolicyFactory::getPolicy(EVertexPolicy policy) {
            switch (policy) {
                case EVertexPolicy::Position:
                    return mPosPolicy;
                case EVertexPolicy::PositionNormal:
                    return mPosNormPolicy;
                case EVertexPolicy::PositionNormalTexCoords:
                    return mPosNormTexCoordsPolicy;
                case EVertexPolicy::PositionNormalTangentTexCoords:
                    return mPosNormTangentTexCoordsPolicy;
                case EVertexPolicy::PositionNormalTangentBitangentTexCoords:
                    return mPosNormTangentBitangentTexCoordsPolicy;
                default: {
                    static TPtrShared<VertexPolicy> ptr;
                    return ptr;
                }
            }
        }

        const TPtrShared<RHIVertexDeclaration> &VertexPolicyFactory::getDeclaration(EVertexPolicy policy) {
            switch (policy) {
                case EVertexPolicy::Position:
                    return mPosDeclaration;
                case EVertexPolicy::PositionNormal:
                    return mPosNormDeclaration;
                case EVertexPolicy::PositionNormalTexCoords:
                    return mPosNormTexCoordsDeclaration;
                case EVertexPolicy::PositionNormalTangentTexCoords:
                    return mPosNormTangentTexCoordsDeclaration;
                case EVertexPolicy::PositionNormalTangentBitangentTexCoords:
                    return mPosNormTangentBitangentTexCoordsDeclaration;
                default: {
                    static TPtrShared<RHIVertexDeclaration> ptr;
                    return ptr;
                }
            }
        }

    }
}