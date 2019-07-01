//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include <IO/IFile.h>
#include <Misc/Buffers.h>
#include <Threading/Mutex.h>
#include <Memory/Allocator.h>
#include <IO/IOutputDevice.h>
#include <Logging/ILogManager.h>

namespace Berserk
{

    /**
     * Default log manager implementation with internal buffer for
     * temporal tasks. Could be used from any thread.
     *
     * @note Multi-threaded
     */
    class CORE_API LogManager : public ILogManager
    {
    public:

        /**
         * Create log manager and push init message
         * @param file Handler of file to write the log
         * @param verbosity Verbosity of this log to filter messages
         */
        explicit LogManager(IFile &file, IOutputDevice& device, ELogVerbosity verbosity = ELogVerbosity::Display);

        ~LogManager() override;

        void addMessage(const char *message, ELogVerbosity verbosity, bool mirrorToOutput) override;

        void addMessage(const char *category, const char *message,
                        ELogVerbosity verbosity, bool mirrorToOutput) override;

        void addPage() override;

        ELogVerbosity getVerbosity() const override { return mVerbosity; }

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

    };

} // namespace Berserk

#endif //BERSERK_LOGMANAGER_H