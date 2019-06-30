//
// Created by Egor Orachyov on 30.06.2019.
//

#include <Serialization/ArchiveFileWriter.h>

namespace Berserk
{

    ArchiveFileWriter::ArchiveFileWriter(const TUniquePtr<IFile> &file, const char *filename)
            : mFile(file), mFileName(filename)
    {

    }

    void ArchiveFileWriter::serialize(const void *source, uint64 bytesToWrite)
    {
        mFile->write(source, bytesToWrite);
    }

} // namespace Berserk