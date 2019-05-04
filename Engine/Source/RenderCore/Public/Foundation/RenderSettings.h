//
// Created by Egor Orachyov on 12.04.2019.
//

#ifndef BERSERK_RENDERSETTINGS_H
#define BERSERK_RENDERSETTINGS_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk::Render
{

    /**
     * General settings for rendering system.
     * (All effects bool fields, with main properties)
     */
    class ENGINE_API RenderSettings
    {
    public:

        /** All effects are disabled */
        RenderSettings()
        {
            mActiveDistantFog = DISABLED;
            mActiveToneMapping = DISABLED;
            mActiveBloomEffect = DISABLED;
            mActiveDirShadowMapping = DISABLED;
            mActiveOmnidirShadowMapping = DISABLED;
        }

        ~RenderSettings() = default;

    public:

        static const bool ACTIVE = true;

        static const bool DISABLED = false;

        /** If should apply in current frame */
        bool mActiveDistantFog : 1;

        /** If should apply in current frame */
        bool mActiveToneMapping : 1;

        /** If should apply in current frame */
        bool mActiveBloomEffect : 1;

        /** If should apply in current frame */
        bool mActiveDirShadowMapping : 1;

        /** If should apply in current frame */
        bool mActiveOmnidirShadowMapping : 1;

    };

} // namespace Berserk::Render

#endif //BERSERK_RENDERSETTINGS_H