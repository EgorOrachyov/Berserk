//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IUNIFORMBUFFER_H
#define BERSERK_IUNIFORMBUFFER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Resource/IResource.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    /**
     * Abstract uniform buffer object for cpu - qpu interconnection
     */
    class GRAPHICS_API IUniformBuffer : public IResource
    {
    public:

        /** Create uniform buffer with chosen binding point, size and writes data in that */
        virtual void create(uint32 bindingPoint, uint32 size, const void *data) = 0;

        /** Update uniform buffer with data */
        virtual void update(uint32 size, const void *data) = 0;

        /** Update uniform buffer only after chosen offset */
        virtual void update(uint32 offset, uint32 size, const void *data) = 0;

        /** Bind uniform buffer to chosen binding point */
        virtual void bind(uint32 bindingPoint) = 0;

        /** Bind uniform buffer to its point */
        virtual void bind() = 0;

        /** Set fixed binding point for uniform buffer */
        virtual void setBindingPoint(uint32 bindingPoint) = 0;

        /** @return uniform buffer binding point */
        virtual uint32 getBindingPoint() = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IUNIFORMBUFFER_H