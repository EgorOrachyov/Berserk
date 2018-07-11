//
// Created by Egor Orachyov on 09.07.2018.
//

#include "Strings/StringTable.h"

namespace Berserk
{

    StringTable::StringTable()
    {

    }

    StringTable::~StringTable()
    {

    }

    void StringTable::init(UINT32 stringsCount)
    {

    }

    void StringTable::destroy()
    {

    }

    void StringTable::set(const WString& source)
    {

    }

    WString& StringTable::get(UINT32 hash) const
    {

    }

    WString& StringTable::get(const WCHAR* source) const
    {

    }

    UINT32 StringTable::getSize() const
    {
        return mSize;
    }

    UINT32 StringTable::getCapacity() const
    {
        return mCapacity;
    }

    const WString* StringTable::getBuffer() const
    {
        return mBuffer;
    }

} // namespace Berserk