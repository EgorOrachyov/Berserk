//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_RENDERSETTINGS_H
#define BERSERK_RENDERSETTINGS_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk::Render
{

    class ENGINE_API RenderSettings
    {
    public:

        static const bool ACTIVE = true;

        static const bool DISABLED = false;

        bool mActiveDistantFog : 1 = DISABLED;

        bool mActiveToneMapping : 1 = DISABLED;

        bool mActiveBloomEffect : 1 = DISABLED;

        bool mActiveDirShadowMapping : 1 = DISABLED;

        bool mActiveOmnidirlShadowMapping : 1 = DISABLED;

    };

} // namespace Berserk::Render

#endif //BERSERK_RENDERSETTINGS_H