/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXDECLARATIONBUILDER_H
#define BERSERK_VERTEXDECLARATIONBUILDER_H

#include <RHI/RHIDescs.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>

namespace Berserk {
    namespace Render {

        class VertexDeclarationBuilder {
        public:

            using Builder = VertexDeclarationBuilder;

            VertexDeclarationBuilder() = default;

            /** Set declaration name for look-up */
            Builder& setName(const CString& name);

            /** Add buffer with specified name */
            Builder& addBuffer(const CString& name);

            /** Add buffer with specified name and elements iterating */
            Builder& addBuffer(const CString& name, EVertexIterating iterating);

            /** Add element to current buffer with specified name and type */
            Builder& addElement(const CString& name, EVertexElementType elementType);

            /** @return Instance declaration */
            const TPtrShared<class VertexDeclaration> &buildShared();

        private:

            void flush();

            friend class VertexDeclaration;

            CString mName;
            uint32 mNextBuffer = 0;
            uint32 mNextElement = 0;
            uint32 mBufferIndex = 0;
            uint32 mBufferStride = 0;
            uint32 mElementsToFlush = 0;
            bool mNeedBufferFlush = false;
            EVertexIterating mBufferIterating = EVertexIterating::PerVertex;
            TMap<CString,uint32> mElementsIdx;
            TMap<CString,uint32> mBuffersIdx;
            TArray<RHIVertexElement> mElements;
            TArray<class VertexBufferInfo> mBuffers;
            TPtrShared<class VertexDeclaration> mInstance;

        };

    }
}

#endif //BERSERK_VERTEXDECLARATIONBUILDER_H