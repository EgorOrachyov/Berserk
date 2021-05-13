/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FILETEXTWRITER_HPP
#define BERSERK_FILETEXTWRITER_HPP

#include <BerserkCore/IO/TextWriter.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/Misc/PtrShared.hpp>

namespace Berserk {

    /**
     * @brief File text writer
     *
     * Simple file wrapper to write textual data into platform fs files.
     */
    class FileTextWriter: public TextWriter {
    public:
        using TextWriter::Write;
        using TextWriter::WriteLine;
        using TextWriter::WriteF;
        using TextWriter::WriteFF;

        /** Create text writer from opened platform file */
        explicit FileTextWriter(Ref<File> file);
        /** Create tex writer for provided file path */
        explicit FileTextWriter(const String &filePath);

        ~FileTextWriter() override = default;

        void Write(size_t symbolsCount, const String::CharType *string) override;
        void Flush() override;
    private:
        Ref<File> mFileHnd;
    };

}

#endif //BERSERK_FILETEXTWRITER_HPP