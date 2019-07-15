//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIUNIFORMBUFFER_H
#define BERSERK_RHIUNIFORMBUFFER_H

#include <RHIResource.h>

namespace Berserk
{

    class GRAPHICS_API RHIUniformBuffer
    {
    public:

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

    };

} // namespace Berserk

#endif //BERSERK_RHIUNIFORMBUFFER_H