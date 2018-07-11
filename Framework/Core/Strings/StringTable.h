//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERKENGINE_STRINGTABLE_H
#define BERSERKENGINE_STRINGTABLE_H

#include "WString.h"

#include "Essential/Types.h"
#include "HashFunctions/CRC32.h"
#include "Containers/ArrayList.h"

namespace Berserk
{

    class StringTable
    {
    public:

        StringTable();
        ~StringTable();

        void init(UINT32 stringsCount = 1024);
        void destroy();

        void set(const WString& source);

        WString& get(UINT32 hash) const;
        WString& get(const WCHAR* source) const;

        UINT32 getSize() const;
        UINT32 getCapacity() const;
        const WString* getBuffer() const;

    private:

        UINT32 mSize;
        UINT32 mCapacity;
        WString* mBuffer;

    };

} // namespace Berserk

#endif //BERSERKENGINE_STRINGTABLE_H