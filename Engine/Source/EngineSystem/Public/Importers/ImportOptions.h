//
// Created by Egor Orachyov on 2019-08-26.
//

#ifndef BERSERK_IMPORTOPTIONS_H
#define BERSERK_IMPORTOPTIONS_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /** Import options for the importer. Control the way resource imported in the engine */
    class ENGINE_API ImportOptions
    {
    public:

        virtual ~ImportOptions() = default;

    };

} // namespace Berserk

#endif //BERSERK_IMPORTOPTIONS_H