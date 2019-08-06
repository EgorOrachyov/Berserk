//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_UUID_H
#define BERSERK_UUID_H

#include <Strings/String.h>

namespace Berserk
{

    /**
     * Unique string id, used for game objects and resources for proper
     * naming for loading/find purposes in run-time mode.
     *
     * @note Object UUID should must be unique among other ids.
     * @note Object UUID must be defined outside the engine and only loaded in run-time mode.
     */
    typedef String UUID;

} // namespace Berserk

#endif //BERSERK_UUID_H
