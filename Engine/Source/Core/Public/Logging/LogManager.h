//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include <IO/IFile.h>
#include <Misc/Buffers.h>
#include <Threading/Mutex.h>
#include <Memory/Alloc.h>
#include <IO/IOutputDevice.h>
#include <Logging/ILogManager.h>
#include <Time/Clock.h>
#include <Strings/StringStatic.h>

namespace Berserk
{

    /**
     * Default log manager implementation with internal buffer for
     * temporal tasks. Could be used from any thread.
     *
     * @note Thread-Safe
     */
    class CORE_API LogManager : public ILogManager
    {
    public:

        /**
         * Create log manager and push init message
         *
         * @param logName Logger name for output
         * @param file Handler of file to write the log
         * @param device Device for default output (when does not mirror to file)
         * @param verbosity Verbosity of this log to filter messages
         */
        explicit LogManager(const char *logName, IFile &file, IOutputDevice& device, ELogVerbosity verbosity = ELogVerbosity::Display);

        ~LogManager() override;

        /** @copydoc ILogManager::addMessage() */
        void addMessage(const char *message, ELogVerbosity verbosity, bool mirrorToOutput) override;

        /** @copydoc ILogManager::addMessage() */
        void addMessage(const char *category, const char *message,
                        ELogVerbosity verbosity, bool mirrorToOutput) override;

        /** @copydoc ILogManager::addPage() */
        void addPage() override;

        /** @copydoc ILogManager::getVerbosity() */
        ELogVerbosity getVerbosity() const override { return mVerbosity; }

        /** @copydoc ILogManager::getLogName() */
        const char* getLogName() const override { return mLogName.get(); }

    protected:

        void addMessageInit();

        void addMessageFinal();

        void writeVerbosity(ELogVerbosity verbosity, char* buffer);

        void writeToFile(int32 written, char* buffer);

    protected:

        volatile ELogVerbosity mVerbosity;

        IFile& mFile;
        IOutputDevice& mDevice;
        uint64 mMessagesNum = 0;
        uint64 mPageNum = 0;
        Mutex mMutex;
        Clock mClock;
        StringStatic<char, '\0', 64> mLogName;


    };

} // namespace Berserk

#endif //BERSERK_LOGMANAGER_H