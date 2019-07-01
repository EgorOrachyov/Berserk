//
// Created by Egor Orachyov on 03.06.2019.
//

#ifndef BERSERK_ARRAYTEST_H
#define BERSERK_ARRAYTEST_H

#include <Math/MathInclude.h>
#include <Containers/TArray.h>
#include <Strings/String.h>
#include <IO/OutputDevice.h>
#include <Serialization/ArchiveFileWriter.h>
#include <Serialization/ArchiveFileReader.h>

using namespace Berserk;

class ArrayTest
{

public:

    static void ArrayTest1()
    {
        OutputDevice::printf("\nTArray\n");

        TArray<int64> array1(2000);
        TArray<int64> array2(1000);

        TList<int64> &list = array1;
        TList<int64> &another = array2;

        for (uint32 i = 0; i < 1400; i++)
        { list.add(Math::random(0.0f, 1000.0f)); }

        for (uint32 i = 0; i < 714; i++)
        { another.add(Math::random(0.0f, 1000.0f)); }

        list.append(another);
        list.sort([](const int64 &a, const int64 &b){
            return a < b;
        });

        int32 i = 0;
        for (int64* e = list.begin(); e != nullptr; e = list.next())
        { OutputDevice::printf("a[%i]=%li\n", i++, *e); }

        OutputDevice::printf("\n\n");
    }

    static void ArrayTest2()
    {
        OutputDevice::printf("\nTArray\n");

        TArray<String> stringArray;

        new(stringArray.addUninitialized()) String("Hello");
        new(stringArray.addUninitialized()) String("World");
        new(stringArray.addUninitialized()) String("Something");
        new(stringArray.addUninitialized()) String("aaa");
        new(stringArray.addUninitialized()) String("aaba");
        new(stringArray.addUninitialized()) String("aaacc");

        stringArray.emplace("it is");
        stringArray.emplace("new");
        stringArray.emplace("feature, which allows");
        stringArray.emplace("to create object in the container");

        stringArray.sort([](const String &a, const String &b){
            return a.length() < b.length();
        });

        uint32 i = 0;
        for (auto s = stringArray.begin(); s != nullptr; s = stringArray.next())
        {
            OutputDevice::printf("a[%i] = %s\n", i++, s->get());
        }
        OutputDevice::printf("\n\n");
    }

    static void ArrayTest3()
    {
        OutputDevice::printf("\nTArray\n");

        Name buffer[] = { Name("Hello"), Name("some"), Name("world"), Name("in"), Name("the") };
        uint32 size = 5;

        TArray<Name> array(buffer, size);

        array.add(Name("cool"));
        array.add(Name("fresh"));
        array.add(Name("universe"));

        array.sort([](const Name& a, const Name& b){ return a <= b; });

        for (uint32 i = 0; i < array.getSize(); i++)
        {
            OutputDevice::printf("a[%i] = %s\n", i, array.get(i).get());
        }
        OutputDevice::printf("\n\n");
    }

    static void ArrayTest4()
    {
        OutputDevice::printf("\nTArray\n");

        Name buffer[] = { Name("Hello"), Name("some"), Name("world"), Name("in"), Name("the") };
        uint32 size = 5;

        char memory[1024];
        TList<Name> *list = new(memory) TArray<Name>();

        list->append(buffer, size);
        for (auto string = list->begin(); string != nullptr; string = list->next())
        {
            OutputDevice::printf("%s\n", string->get());
        }
        delete list;

        OutputDevice::printf("\n\n");
    }

    static void ArrayTest5()
    {
        String names[] = { String("Alex"), String("Simon"), String("Ramon"), String("Igrit") };

        TArray<String> array;

        array.add(names[0]);
        array.add(names[1]);
        array.add(names[2]);
        array.add(names[3]);

        array.append(names, 4);

        array.emplace("Viktor");
        array.emplace("Sandor");

        for (auto s = array.begin(); s != nullptr; s = array.next())
        {
            OutputDevice::printf("%s \n", s->get());
        }

        TArray<String> another = array;

        for (auto s = another.begin(); s != nullptr; s = another.next())
        {
            OutputDevice::printf("%s \n", s->get());
        }

        for (auto s = array.begin(); s != nullptr; s = array.next())
        {
            OutputDevice::printf("%s \n", s->get());
        }
    }

    static void ArrayTest6()
    {
        TArray<Name> names;

        names.emplace("Banana");
        names.emplace("Apple");
        names.emplace("Orange");
        names.emplace("Lime");

        const char filename[] = "fruitsArray.bin";
        PlatformFile file(filename, false, true);

        ArchiveFileWriter archive(file, filename);

        archive << names;
    }

    static void ArrayTest7()
    {
        const char filename[] = "fruitsArray.bin";
        PlatformFile file(filename);
        ArchiveFileReader archive(file, filename);

        TArray<Name> names;
        archive >> names;

        for (auto string = names.begin(); string != nullptr; string = names.next())
        {
            OutputDevice::printf("String: '%s'\n", string->get());
        }

        OutputDevice::printf("Archive: name: '%s' size: %u \n",
                             archive.getFilename().get(), archive.getSize());
    }

    static void run()
    {
        //ArrayTest1();
        //ArrayTest2();
        //ArrayTest3();
        //ArrayTest4();
        //ArrayTest5();
        //ArrayTest6();
        ArrayTest7();

    }

};


#endif //BERSERK_ARRAYTEST_H
