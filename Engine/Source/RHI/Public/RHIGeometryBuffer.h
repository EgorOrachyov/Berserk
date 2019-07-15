//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIGEOMETRYBUFFER_H
#define BERSERK_RHIGEOMETRYBUFFER_H

#include <RHIResource.h>
#include <RHIDefinitions.h>

namespace Berserk
{

    class RHIGeometryBuffer : public RHIResource
    {
    public:

        virtual ~RHIGeometryBuffer() = default;

        /** Request hardware to draw this geometry */
        virtual void draw() = 0;

        /** Draw specified number of elements */
        virtual void draw(uint32 numOfElements) = 0;

        /** @return Geometry primitive type */
        virtual EPrimitiveType getPrimitiveType() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_RHIGEOMETRYBUFFER_H