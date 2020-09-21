/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FILE_HPP
#define BERSERK_FILE_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/String/TString.hpp>

namespace Berserk {

    enum class EFileStatus {
        NoError = 0,
        FailedRead,
        FailedWrite,
        FailedOffset
    };

    enum class EFileMode {
        Read = 0,
        Write
    };

    class File {
    public:
        virtual ~File() = default;
        virtual EFileStatus offset(uint64 newPosition) = 0;
        virtual EFileStatus read(void* destination, uint64 bytesToRead) const = 0;
        virtual EFileStatus write(const void* source, uint64 bytesToWrite) = 0;
        virtual EFileMode getFileMode() const = 0;
        virtual const String& getOpenName() const = 0;
        virtual uint64 getSize() const = 0;
        virtual uint64 getPosition() const = 0;
        virtual bool canRead() const { return getFileMode() == EFileMode::Read; }
        virtual bool canWrite() const { return getFileMode() == EFileMode::Write; }
    };

}

#endif //BERSERK_FILE_HPP