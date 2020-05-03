/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/VertexPolicy.h>
#include <RHI/RHIDevice.h>
#include <LogMacro.h>

namespace Berserk {

    uint32 VertexInput::getVertexElementSize(EVertex element) {
        switch (element) {
            case EVertex::Position:
                return sizeof(float) * 3;
            case EVertex::Normal:
                return sizeof(float) * 3;
            case EVertex::Tangent:
                return sizeof(float) * 3;
            case EVertex::Bitangent:
                return sizeof(float) * 3;
            case EVertex::TexCoords:
                return sizeof(float) * 2;
            case EVertex::Color:
                return sizeof(float) * 3;
            default:
                return 0;
        }
    }

    const char* VertexInput::getVertexElementName(EVertex element) {
        switch (element) {
            case EVertex::Position:
                return "Position";
            case EVertex::Normal:
                return "Normal";
            case EVertex::Tangent:
                return "Tangent";
            case EVertex::Bitangent:
                return "Bitangent";
            case EVertex::TexCoords:
                return "TexCoords";
            case EVertex::Color:
                return "Color";
            default:
                return "Undefined";
        }
    }

    EVertexElementType VertexInput::getVertexElementType(Berserk::EVertex element) {
        switch (element) {
            case EVertex::Position:
                return EVertexElementType::Float3;
            case EVertex::Normal:
                return EVertexElementType::Float3;
            case EVertex::Tangent:
                return EVertexElementType::Float3;
            case EVertex::Bitangent:
                return EVertexElementType::Float3;
            case EVertex::TexCoords:
                return EVertexElementType::Float2;
            case EVertex::Color:
                return EVertexElementType::Float3;
            default:
                return EVertexElementType::Unknown;
        }
    }

    VertexPolicy::VertexPolicy(const TArrayStatic<EVertex> &elements) : VertexPolicy() {
        uint32 offset = 0;
        TEnumMask<EVertex> mask;

        uint32 location = 0;

        for (auto element: elements) {
            auto index = (uint32) element;
            mElementsList.add(element);
            mElementsOffsets[index] = offset;
            mElementsOffsets[index] = location;

            offset += VertexInput::getVertexElementSize(element);
            mask.setFlag(element, true);
            location += 1;
        }

        mStride = offset;
        mVertexInput = VertexInput(mask);
    }

    VertexPolicy::VertexPolicy() {
        mElementsOffsets.resize(TArrayStatic<EVertex>::CAPACITY);
        mElementsLocations.resize(TArrayStatic<EVertex>::CAPACITY);
    }

    void VertexPolicy::setStride(uint32 stride) {
        mStride = stride;
    }

    void VertexPolicy::addAttribute(EVertex element, uint32 offset, uint32 location) {
        auto& input = getInput();

        if (!input.hasElement(element)) {
            auto index = (uint32) element;
            mElementsList.add(element);
            mElementsOffsets[index] = offset;
            mElementsOffsets[index] = location;

            auto mask = input.getMask();
            mask.setFlag(element, true);
            mVertexInput = VertexInput(mask);
        }
    }


    uint32 VertexPolicy::getOffset(EVertex element) const {
        auto& input = getInput();

        if (!input.hasElement(element))
            return 0;

        auto index = (uint32) element;
        return mElementsOffsets[index];
    }

    void VertexPolicy::getVertexDeclarationDesc(RHIVertexDeclarationDesc &desc, uint32 buffer) const {
        desc.ensureToAdd(getAttributesCount());

        for (auto element: mElementsList) {
            auto index = (uint32) element;
            auto& attribute = desc.emplace();
            attribute.buffer = buffer;
            attribute.iterating = EVertexIterating::PerVertex;
            attribute.location = mElementsLocations[index];
            attribute.offset = mElementsOffsets[index];
            attribute.stride = mStride;
            attribute.type = VertexInput::getVertexElementType(element);
        }
    }

    VertexPolicyFactory::VertexPolicyFactory() {
        using EVX = EVertex;

        mPosPolicy = TPtrShared<VertexPolicy>::make( TArrayStatic<EVertex>{ EVX::Position } );
        mPosNormPolicy = TPtrShared<VertexPolicy>::make( TArrayStatic<EVertex>{ EVX::Position, EVX::Normal } );
        mPosNormTexCoordsPolicy = TPtrShared<VertexPolicy>::make( TArrayStatic<EVertex>{ EVX::Position, EVX::Normal, EVX::TexCoords } );
        mPosNormTangentTexCoordsPolicy = TPtrShared<VertexPolicy>::make( TArrayStatic<EVertex>{ EVX::Position, EVX::Normal, EVX::Tangent, EVX::TexCoords } );
        mPosNormTangentBitangentTexCoordsPolicy = TPtrShared<VertexPolicy>::make( TArrayStatic<EVertex>{ EVX::Position, EVX::Normal, EVX::Tangent, EVX::Bitangent, EVX::TexCoords } );

        auto& device = RHIDevice::getSingleton();
        RHIVertexDeclarationDesc declarationDesc;

        mPosPolicy->getVertexDeclarationDesc(declarationDesc);
        mPosDeclaration = device.createVertexDeclaration(declarationDesc);

        declarationDesc.clear();

        mPosNormPolicy->getVertexDeclarationDesc(declarationDesc);
        mPosNormDeclaration = device.createVertexDeclaration(declarationDesc);

        declarationDesc.clear();

        mPosNormTexCoordsPolicy->getVertexDeclarationDesc(declarationDesc);
        mPosNormTexCoordsDeclaration = device.createVertexDeclaration(declarationDesc);

        declarationDesc.clear();

        mPosNormTangentTexCoordsPolicy->getVertexDeclarationDesc(declarationDesc);
        mPosNormTangentTexCoordsDeclaration = device.createVertexDeclaration(declarationDesc);

        declarationDesc.clear();

        mPosNormTangentBitangentTexCoordsPolicy->getVertexDeclarationDesc(declarationDesc);
        mPosNormTangentBitangentTexCoordsDeclaration = device.createVertexDeclaration(declarationDesc);

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