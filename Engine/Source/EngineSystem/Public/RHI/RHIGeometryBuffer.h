//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIGEOMETRYBUFFER_H
#define BERSERK_RHIGEOMETRYBUFFER_H

#include <RHI/RHIResource.h>
#include <Rendering/Definitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIGeometryBuffer : public RHIResource
    {
    public:

        ~RHIGeometryBuffer() override = default;

        /** Draw only specified number of indices/elements [depends on created buffer] */
        virtual void draw(uint32 numOfElements) = 0;

        /** Draw specified number of indices, with specified offset from the beginning of the buffer */
        virtual void draw(uint32 numOfIndices, uint32 offsetBuffer) = 0;

        /** @return Geometry primitive type */
        virtual EPrimitiveType getPrimitiveType() const = 0;

    };

    typedef TSharedPtr<RHIGeometryBuffer> RHIGeometryBufferRef;

} // namespace Berserk

#endif //BERSERK_RHIGEOMETRYBUFFER_H