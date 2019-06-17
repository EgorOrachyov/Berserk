//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_RESOURCESTEST_H
#define BERSERK_RESOURCESTEST_H

#include <Containers/TArray.h>
#include <Resource/TUniquePtr.h>

using namespace Berserk;

class ResourcesTest
{
public:

    static void UniquePointerTest()
    {
        typedef TArray<CName> NamesArray;
        IAllocator& allocator = Allocator::get();
        TUniquePtr<NamesArray> array(new(allocator.allocate(sizeof(NamesArray))) NamesArray(), &allocator);

        array->emplace("Hello");
        array->emplace("How are you");

        TUniquePtr<NamesArray> other = array;

        printf("Is null?: %i\n", array.isNull());

        other->emplace("It is move semantic");
        other->emplace("Because it is unique pointer");

        printf("Is null?: %i\n", other.isNull());
    }

    static void run()
    {
        UniquePointerTest();
    }

};


#endif //BERSERK_RESOURCESTEST_H
