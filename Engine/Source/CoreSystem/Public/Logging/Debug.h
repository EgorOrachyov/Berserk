//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_DEBUG_H
#define BERSERK_DEBUG_H

#include <Logging/LogManager.h>
#include <IO/PlatformFileDev.h>
#include <IO/CachedFileWriter.h>

namespace Berserk
{

    /** Debug log static instance provider */
    class CORE_API Debug
    {
    public:

        /** @return Debug global log manager instance */
        static LogManager& log()
        {
            static PlatformFileDev fileDev("BerserkDebug.log", false, true);
            static CachedFileWriter fileWriter(fileDev, KiB * 10);
            static LogManager logManager(fileWriter, ELogVerbosity::Display);
            return logManager;
        }

    };

} // namespace Berserk

#endif //BERSERK_DEBUG_H