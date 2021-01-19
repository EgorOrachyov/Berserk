/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXFILE_HPP
#define BERSERK_UNIXFILE_HPP

#include <BerserkCore/Platform/File.hpp>

namespace Berserk {
    namespace Platform {

        class UnixFile: public File {
        public:
            UnixFile(const String& path, Mode mode);
            UnixFile(UnixFile&& other) noexcept;
            ~UnixFile() override;

            void Close() override;
            void Flush() override;
            void Seek(uint64 position) override;

            bool IsOpen() const override;
            bool IsEof() const override;

            size_t GetSize() const override;
            size_t GetPosition() const override;

            Mode GetAccessMode() const override;
            const String &GetFilename() const override;

            size_t ReadBytes(void *destination, size_t size) override;
            size_t WriteBytes(const void *source, size_t size) override;

            static Ref<UnixFile> Create(const String& path, Mode mode);

        protected:
            void OnReleased() const override;

        private:
            String mName;
            FILE* mHND = nullptr;
            Mode mMode;
        };

    }
}

#endif //BERSERK_UNIXFILE_HPP