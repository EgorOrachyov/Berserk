/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <VertexDeclarationCache.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        VertexDeclarationBuilder& VertexDeclarationBuilder::setName(const CString &name) {
            mName = name;
            return *this;
        }

        VertexDeclarationBuilder& VertexDeclarationBuilder::addBuffer(const CString &name) {
            return addBuffer(name, EVertexIterating::PerVertex);
        }

        VertexDeclarationBuilder& VertexDeclarationBuilder::addBuffer(const CString &name, EVertexIterating iterating) {
            flush();

            mBuffersIdx[name] = mNextBuffer;
            mBufferIndex = mNextBuffer;
            mBufferStride = 0;
            mBufferIterating = iterating;
            mNeedBufferFlush = true;

            auto& buffer = mBuffers.emplace();
            buffer.index = mBufferIndex;
            buffer.iterating = mBufferIterating;
            buffer.name = name;

            mNextBuffer += 1;
            return *this;
        }

        VertexDeclarationBuilder& VertexDeclarationBuilder::addElement(const CString &name, EVertexElementType elementType) {
            mElementsIdx[name] = mNextElement;

            auto& element = mElements.emplace();
            element.buffer = mBufferIndex;
            element.iterating = mBufferIterating;
            element.location = mNextElement;
            element.offset = mBufferStride;
            element.type = elementType;

            auto& buffer = mBuffers[mBufferIndex];
            buffer.elements.add(name);

            mBufferStride += RHIDefinitionsUtil::getVertexElementSize(elementType);
            mNextElement += 1;
            mElementsToFlush += 1;

            return *this;
        }

        const TPtrShared<class VertexDeclaration>& VertexDeclarationBuilder::buildShared() {
            if (mInstance.isNotNull())
                return mInstance;

            flush();

            mInstance = TPtrShared<VertexDeclaration>::make(*this);
            return mInstance;
        }

        void VertexDeclarationBuilder::flush() {
            if (mNeedBufferFlush) {
                BERSERK_COND_ERROR_RET(mElementsToFlush > 0, "Buffer has no elements: %s", mName.data());

                auto& buffer = mBuffers[mBufferIndex];
                buffer.stride = mBufferStride;

                for (auto& element: buffer.elements) {
                    auto i = mElementsIdx[element];
                    auto& e = mElements[i];

                    e.stride = mBufferStride;
                }

                mNeedBufferFlush = false;
                mElementsToFlush = 0;
            }
        }

    }
}