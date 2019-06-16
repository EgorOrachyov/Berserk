//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_ARCHIVE_H
#define BERSERK_ARCHIVE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>

namespace Berserk
{

    class CORE_API Archive
    {
    public:

        virtual ~Archive() = default;

        virtual void serialize(const void* source, uint64 bytesToWrite) = 0;

        virtual void unserialize(void* destination, uint64 bytesToRead) = 0;

        template <typename T>
        friend Archive& operator<<(Archive& archive, const T& arg)
        {
            archive.serialize(&arg, sizeof(T));
        }

        template <typename T>
        friend Archive& operator>>(Archive& archive, const T& arg)
        {
            archive.unserialize(&arg, sizeof(T));
        }

    };

} // namespace Berserk

#endif //BERSERK_ARCHIVE_H