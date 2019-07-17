//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_RESOURCESTEST_H
#define BERSERK_RESOURCESTEST_H

#include <Containers/TArray.h>
#include <Resource/TUniquePtr.h>
#include <Resource/TSharedPtr.h>
#include <Resource/TWeakPtr.h>

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

        ptr1->emplace("Tests");
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

    static void PointersTest1()
    {
        typedef TArray<String> StringsList;
        typedef TSharedPtr<StringsList> SharedRef;
        typedef TWeakPtr<StringsList> WeakRef;

        IAllocator& allocator = Allocator::get();

        SharedRef shared(allocator);
        shared->emplace("1");
        shared->emplace("12");
        shared->emplace("123");

        WeakRef weak = shared;

        {
            auto ptr = weak.lock();
            if (!ptr.isNull())
            {
                for (auto s = ptr->begin(); s != nullptr; s = ptr->next())
                {
                    OutputDevice::printf("%s \n", s->get());
                }
            }
        }
    }

    static void PointersTest2()
    {
        typedef TArray<String> StringsList;
        typedef TSharedPtr<StringsList> SharedRef;
        typedef TWeakPtr<StringsList> WeakRef;

        IAllocator& allocator = Allocator::get();

        WeakRef weak;

        {
            SharedRef shared(allocator);
            shared->emplace("1");
            shared->emplace("12");
            shared->emplace("123");

            weak = shared;
        }

        {
            auto ptr = weak.lock();
            if (!ptr.isNull())
            {
                for (auto s = ptr->begin(); s != nullptr; s = ptr->next())
                {
                    OutputDevice::printf("%s \n", s->get());
                }
            }
            else
            {
                OutputDevice::printf("Resource is freed \n");
            }
        }
    }

    static void run()
    {
        //UniquePointerTest();
        //SharedPointerTest();
        //PointersTest1();
        PointersTest2();
    }

};


#endif //BERSERK_RESOURCESTEST_H
