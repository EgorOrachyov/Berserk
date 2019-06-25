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
        typedef TArray<uint64> NumsList;
        
        TUniquePtr<NumsList> array(Allocator::get());

        array->emplace(1);
        array->emplace(2);

        TUniquePtr<NumsList> other = array;

        printf("Is null?: %i\n", array.isNull());

        other->emplace(3);
        other->emplace(4);

        printf("Is null?: %i\n", other.isNull());
    }

    static void SharedPointerTest()
    {
        typedef TArray<String> StringsList;

        TSharedPtr<StringsList> ptr1(Allocator::get());

        ptr1->emplace("Test");
        ptr1->emplace("Some");

        TSharedPtr<StringsList> ptr2 = ptr1;

        ptr2->emplace("Share ptr");
        ptr2->emplace("Features");

        TSharedPtr<StringsList> ptr3(Allocator::get());

        ptr3->emplace("Ptr1 and");
        ptr3->emplace("Ptr2 has");
        ptr3->emplace("The same resource");

        ptr1 = ptr3;
        ptr2 = ptr3;

        /** Now first array is freed */
    }

    static void run()
    {
        UniquePointerTest();
        SharedPointerTest();
    }

};


#endif //BERSERK_RESOURCESTEST_H
