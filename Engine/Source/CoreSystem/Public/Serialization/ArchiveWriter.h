//
// Created by Egor Orachyov on 16.06.2019.
//

#ifndef BERSERK_ARCHIVEWRITER_H
#define BERSERK_ARCHIVEWRITER_H

#include <IO/IFile.h>
#include <Serialization/Archive.h>

namespace Berserk
{

    class ArchiveWriter : public Archive
    {
    public:

        ArchiveWriter(IFile& file);

        ~ArchiveWriter() override;

        void serialize(const void *source, uint64 bytesToWrite) override;

        void unserialize(void *destination, uint64 bytesToRead) override;

    protected:



    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEWRITER_H