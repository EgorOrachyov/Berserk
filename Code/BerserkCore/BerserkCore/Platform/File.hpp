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
        virtual EFileStatus Offset(uint64 newPosition) = 0;
        virtual EFileStatus Read(void* destination, uint64 bytesToRead) const = 0;
        virtual EFileStatus Write(const void* source, uint64 bytesToWrite) = 0;
        virtual EFileMode GetFileMode() const = 0;
        virtual const String& GetOpenName() const = 0;
        virtual uint64 GetSize() const = 0;
        virtual uint64 GetPosition() const = 0;
        virtual bool CanRead() const { return GetFileMode() == EFileMode::Read; }
        virtual bool CanWrite() const { return GetFileMode() == EFileMode::Write; }
    };

}

#endif //BERSERK_FILE_HPP