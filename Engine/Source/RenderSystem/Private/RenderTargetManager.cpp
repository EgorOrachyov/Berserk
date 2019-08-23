//
// Created by Egor Orachyov on 2019-08-23.
//

#include "RenderTargetManager.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    RenderTargetManager::~RenderTargetManager()
    {
        DEBUG_LOG_DISPLAY("RenderTargetManager: destroy");
    }

    const Size RenderTargetManager::DEFAULT_LIMIT = Size(10000,10000);

} // namespace Berserk