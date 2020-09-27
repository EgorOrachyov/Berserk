/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORMFILE_HPP
#define BERSERK_PLATFORMFILE_HPP

#include <BerserkCore/Platform/File.hpp>

namespace Berserk {

    class PlatformFile final: public File {
    public:
        PlatformFile(String name, EFileMode mode);
        ~PlatformFile() override;
        EFileStatus Offset(uint64 newPosition) override;
        EFileStatus Read(void *destination, uint64 bytesToRead) const override;
        EFileStatus Write(const void *source, uint64 bytesToWrite) override;
        uint64 GetSize() const override;
        uint64 GetPosition() const override;
        EFileMode GetFileMode() const override;
        const String &GetOpenName() const override;
        bool isSuccessfullyOpened() const { return mFileHnd != nullptr; }
    private:
        String mOpenName;
        FILE* mFileHnd;
        EFileMode mMode;
    };

}

#endif //BERSERK_PLATFORMFILE_HPP