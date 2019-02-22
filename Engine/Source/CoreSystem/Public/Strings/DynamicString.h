//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_DYNAMICSTRING_H
#define BERSERK_DYNAMICSTRING_H

#include "Public/Strings/StringPool.h"

namespace Berserk
{

    template <typename T>
    class DynamicString
    {
    public:

        DynamicString();

        explicit DynamicString(const T* source);

        DynamicString(const DynamicString& source);

    public:

        void empty();

        void operator += (const T *source);

        void operator += (const DynamicString& source);

        DynamicString operator = (const T *source);

        DynamicString operator = (const DynamicString& source);

        uint32 length() const;

        T* get() const;

    private:

        T* mBuffer;                     // String data
        StringPool::PoolNode* mNode;    // String info

    };

} // namespace Berserk

#endif //BERSERK_DYNAMICSTRING_H