/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXSTREAMS_H
#define BERSERK_VERTEXSTREAMS_H

#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Math/Vec4f.h>
#include <RenderResources/VertexArray.h>

namespace Berserk {
    namespace Render {

        /** Provides access to single element */
        class ElementStream {
        public:

            ElementStream() = default;

            ElementStream(EVertexElementType type, BinaryData& data, uint32 offset, uint32 stride, uint32 baseOffset = 0) {
                this->type = type;
                this->data = &data;
                this->offset = offset;
                this->stride = stride;
                this->elementSize = RHIDefinitionsUtil::getVertexElementSize(type);
                this->baseOffset = baseOffset;
                this->canResize = true;
            }

            ElementStream(EVertexElementType type, RHIVertexBuffer* buffer, BinaryData& data, uint32 offset, uint32 stride, uint32 range, uint32 baseOffset = 0) {
                this->type = type;
                this->buffer = buffer;
                this->data = &data;
                this->offset = offset;
                this->stride = stride;
                this->elementSize = RHIDefinitionsUtil::getVertexElementSize(type);
                this->range = range;
                this->baseOffset = baseOffset;
            }

            void ensureToAdd(uint32 count) {
                if (canResize) {
                    auto bufferSize = count * stride + baseOffset;
                    if (data->size() < bufferSize) {
                        data->resize(bufferSize);
                    }
                }
            }

            void addElement(float element) {
                BERSERK_COND_ERROR_RET(type == EVertexElementType::Float1, "Invalid type");
                BERSERK_COND_ERROR_RET(checkCanWrite(), "Out of range value");
                write(&element);
            }

            void addElement(const Vec2f& element) {
                BERSERK_COND_ERROR_RET(type == EVertexElementType::Float2, "Invalid type");
                BERSERK_COND_ERROR_RET(checkCanWrite(), "Out of range value");
                write(&element);
            }

            void addElement(const Vec3f& element) {
                BERSERK_COND_ERROR_RET(type == EVertexElementType::Float3, "Invalid type");
                BERSERK_COND_ERROR_RET(checkCanWrite(), "Out of range value");
                write(&element);
            }

            void addElement(const Vec4f& element) {
                BERSERK_COND_ERROR_RET(type == EVertexElementType::Float4, "Invalid type");
                BERSERK_COND_ERROR_RET(checkCanWrite(), "Out of range value");
                write(&element);
            }

            template <typename T>
            void addElements(const std::initializer_list<T> &elements) {
                for (auto& e: elements)
                    addElement(e);
            }

            template <typename T>
            friend ElementStream& operator<<(ElementStream& stream, const T& element) {
                stream.addElement(element);
                return stream;
            }

        private:

            void write(const void* what) {
                if (canResize)
                    ensureToAdd(1);

                uint8* where = data->data() + baseOffset + offset;
                Memory::copy(where,what, elementSize);
                baseOffset += stride;
            }

            bool checkCanWrite() {
                return canResize || ((baseOffset + offset + elementSize) <= range);
            }

            EVertexElementType type = EVertexElementType::Unknown;
            RHIVertexBuffer* buffer = nullptr;
            BinaryData* data = nullptr;
            uint32 offset = 0;
            uint32 stride = 0;
            uint32 elementSize = 0;
            uint32 baseOffset = 0;
            uint32 range = 0;
            bool canResize = false;
        };

        /** Provides access to single index value */
        class IndexStream {
        public:

            IndexStream() = default;

            IndexStream(EIndexType type, BinaryData& data, uint32 baseOffset = 0) {
                this->type = type;
                this->data = &data;
                this->indexSize = RHIDefinitionsUtil::getIndexSize(type);
                this->baseOffset = baseOffset;
                this->canResize = true;
            }

            IndexStream(EIndexType type, RHIIndexBuffer* buffer, BinaryData& data, uint32 range, uint32 baseOffset = 0) {
                this->type = type;
                this->buffer = buffer;
                this->data = &data;
                this->indexSize = RHIDefinitionsUtil::getIndexSize(type);
                this->range = range;
                this->baseOffset = baseOffset;
            }

            void ensureToAdd(uint32 count) {
                if (canResize) {
                    auto bufferSize = count * indexSize + baseOffset;
                    if (data->size() < bufferSize) {
                        data->resize(bufferSize);
                    }
                }
            }

            void addElement(uint32 element) {
                BERSERK_COND_ERROR_RET(type == EIndexType::Uint32, "Invalid type");
                BERSERK_COND_ERROR_RET(checkCanWrite(), "Out of range value");
                write(&element);
            }

            template <typename T>
            void addElements(const std::initializer_list<T> &elements) {
                for (auto& e: elements)
                    addElement(e);
            }

            template <typename T>
            friend IndexStream& operator<<(IndexStream& stream, const T& element) {
                stream.addElement(element);
                return stream;
            }

        private:

            void write(const void* what) {
                if (canResize)
                    ensureToAdd(1);

                uint8* where = data->data() + baseOffset;
                Memory::copy(where,what,indexSize);
                baseOffset += indexSize;
            }

            bool checkCanWrite() {
                return canResize || ((baseOffset + indexSize) <= range);
            }

            EIndexType type = EIndexType::Unknown;
            RHIIndexBuffer* buffer = nullptr;
            BinaryData* data = nullptr;
            uint32 indexSize = 0;
            uint32 baseOffset = 0;
            uint32 range = 0;
            bool canResize = false;
        };

    }
}

#endif //BERSERK_VERTEXSTREAMS_H
