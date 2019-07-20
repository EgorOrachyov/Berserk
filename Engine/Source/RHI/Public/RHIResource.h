//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHIRESOURCE_H
#define BERSERK_RHIRESOURCE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>
#include <Resource/TSharedPtr.h>
#include <Resource/IResource.h>

namespace Berserk
{

    /** Base class for all RHI resources */
    class GRAPHICS_API RHIResource : public IResource
    {
    public:

        /** Support virtual destruction in shared pointers */
        virtual ~RHIResource() = default;

    };

} // namespace Berserk

#endif //BERSERK_RHIRESOURCE_H