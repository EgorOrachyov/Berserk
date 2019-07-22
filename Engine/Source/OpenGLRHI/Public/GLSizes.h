//
// Created by Egor Orachyov on 2019-07-22.
//

#ifndef BERSERK_GLSIZES_H
#define BERSERK_GLSIZES_H

#include <Rendering/Definitions.h>
#include <Logging/DebugLogMacros.h>

#ifndef COMPONENT_SIZE
#define COMPONENT_SIZE(enum, glenum) \
    case (enum): \
        return glenum;
#endif

namespace Berserk
{
    class GLSizes
    {
    public:

        static uint32 getComponentSize(EStorageFormat value)
        {
            switch (value)
            {
                COMPONENT_SIZE(EStorageFormat::SF_R8, 4);
                COMPONENT_SIZE(EStorageFormat::SF_RGB8, 4);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA8, 4);
                COMPONENT_SIZE(EStorageFormat::SF_RGB16F, 8);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA16F, 8);
                COMPONENT_SIZE(EStorageFormat::SF_RGB32F, 12);
                COMPONENT_SIZE(EStorageFormat::SF_RGBA32F, 16);
                COMPONENT_SIZE(EStorageFormat::SF_DEPTH24, 4);
                COMPONENT_SIZE(EStorageFormat::SF_DEPTH24_STENCIL8, 4);

                default:
                    DEBUG_LOG_WARNING("GLStorageFormat: invalid enum");
            }

            return 0;
        }

    };

} // namespace Berserk

#undef COMPONENT_SIZE

#endif //BERSERK_GLSIZES_H