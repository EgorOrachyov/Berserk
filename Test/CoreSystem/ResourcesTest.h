//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_RESOURCESTEST_H
#define BERSERK_RESOURCESTEST_H

#include <Containers/TArray.h>
#include <Resource/TUniquePtr.h>
#include <Resource/TSharedPtr.h>

using namespace Berserk;

class ResourcesTest
{
public:

    static void UniquePointerTest()
    {
        typedef TArray<uint64> NamesArray;
        IAllocator& allocator = Allocator::get();
        TUniquePtr<NamesArray> array(&allocator);

        array->emplace(1);
        array->emplace(2);

        TUniquePtr<NamesArray> other = array;

        printf("Is null?: %i\n", array.isNull());

        other->emplace(3);
        other->emplace(4);

        printf("Is null?: %i\n", other.isNull());
    }

    static void run()
    {
        UniquePointerTest();
    }

};


#endif //BERSERK_RESOURCESTEST_H
