//
// Created by Egor Orachyov on 30.05.2018.
//

#ifndef BERSERKENGINE_CAST_H
#define BERSERKENGINE_CAST_H

#include "Logging/LogMessages.h"

namespace Berserk
{

    class Cast
    {

    public:

        template <typename Target, typename Source>
        static Target* safeDynamic(Source* source) {
            auto result = dynamic_cast<Target*>(source);
            if (result == nullptr) {
                WARNING("Cast::safeDynamic: cannot cast pointer %p", source);
            }
            return result;
        }

        template <typename Target, typename Source>
        static Target* safeStatic(Source* source) {
            auto result = static_cast<Target*>(source);
            if (result == nullptr) {
                WARNING("Cast::safeDynamic: cannot cast pointer %p", source);
            }
            return result;
        }

    };

} // namespace Berserk

#endif //BERSERKENGINE_CAST_H
