//
// Created by Egor Orachyov on 01.07.2019.
//

#include <Serialization/ArchiveFileReader.h>

namespace Berserk
{

    ArchiveFileReader::ArchiveFileReader(IFile &file, const char *filename)
            : mFile(file), mFileName(filename)
    {

    }

    void ArchiveFileReader::deserialize(void *destination, uint64 bytesToRead)
    {
        mFile.read(destination, bytesToRead);
    }

} // namespace Berserk