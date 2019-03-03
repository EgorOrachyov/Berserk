//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLUNIFORMBUFFER_H
#define BERSERK_GLUNIFORMBUFFER_H

#include "Strings/StaticString.h"
#include "Platform/IUniformBuffer.h"

namespace Berserk
{

    /**
     * OpenGL platform implementation for uniform buffer object
     */
    class GRAPHICS_API GLUniformBuffer : public IUniformBuffer
    {
    public:

        /** @copydoc IResource::initialize() */
        void initialize(const char* name) override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char* getName() override;

    public:

        /** @copydoc IUniformBuffer::create() */
        void create(uint32 bindingPoint, uint32 size, const void *data) override;

        /** @copydoc IUniformBuffer::update(size,data) */
        void update(uint32 size, const void *data) override;

        /** @copydoc IUniformBuffer::update(offset,size,data) */
        void update(uint32 offset, uint32 size, const void *data) override;

        /** @copydoc IUniformBuffer::bind(bindingPoint) */
        void bind(uint32 bindingPoint) override;

        /** @copydoc IUniformBuffer::bind() */
        void bind() override;

        /** @copydoc IUniformBuffer::setBindingPoint() */
        void setBindingPoint(uint32 bindingPoint) override;

        /** @copydoc IUniformBuffer::getBindingPoint() */
        uint32 getBindingPoint() override;

        /** @copydoc IUniformBuffer::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

    private:

        uint32 mBufferID;
        uint32 mBufferSize;
        uint32 mBindingPoint;
        uint32 mReferenceCount;
        CName  mResourceName;

    };

} // namespace Berserk




#endif //BERSERK_GLUNIFORMBUFFER_H
