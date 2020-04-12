/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LOGS_H
#define BERSERK_LOGS_H

#include <IO/ILog.h>
#include <TPtrUnique.h>
#include <Platform/IFile.h>
#include <Platform/IMutex.h>

namespace Berserk {

    /**
     * @brief Std logger
     * Logger to out info in default output stream
     * @note Thread-Safe
     */
    class LogStdout final : public ILog {
    public:
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;
    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Info;
    };

    /**
     * @brief File logger
     * Logger to out info in some file, accessed via file handle
     * @note Thread-safe
     */
    class LogFile final : public ILog {
    public:
        LogFile(TPtrUnique<IFile>& file);
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;
    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Info;
        TPtrUnique<IFile> mLogFile;
        TPtrUnique<IMutex> mAccessMutex;
    };

}

#endif //BERSERK_LOGS_H