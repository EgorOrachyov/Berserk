//
// Created by Egor Orachyov on 01.06.2018.
//

#ifndef BERSERKENGINE_CLASSTESTING_H
#define BERSERKENGINE_CLASSTESTING_H

#include "../Core/Essential/Types.h"

#include "../Core/Memory/StaticPoolAllocator.h"
#include "../Core/Memory/StackAllocator.h"

#include "../Core/Containers/LinkedList.h"

#include "../Core/Strings/CStaticString.h"

void TestVariableSize()
{
    printf("Type:         Size:\n");
    printf("Var: int8      %li \n", sizeof(Berserk::int8));
    printf("Var: int16     %li \n", sizeof(Berserk::int16));
    printf("Var: int32     %li \n", sizeof(Berserk::int32));
    printf("Var: int64     %li \n", sizeof(Berserk::int64));
    printf("Var: uint8     %li \n", sizeof(Berserk::uint8));
    printf("Var: uint16    %li \n", sizeof(Berserk::uint16));
    printf("Var: uint32    %li \n", sizeof(Berserk::uint32));
    printf("Var: uint64    %li \n", sizeof(Berserk::uint64));
    printf("Var: float32   %li \n", sizeof(Berserk::float32));
    printf("Var: float64   %li \n", sizeof(Berserk::float64));
    printf("Ptr: int32     %li \n", sizeof(Berserk::int32*));
    printf("Ptr: int64     %li \n", sizeof(Berserk::int64*));

    printf("\n\n");
}

void TestClassSize()
{
    printf("Pool Allocator:  %4li\n", sizeof(Berserk::StaticPoolAllocator));
    printf("Stack Allocator: %4li\n", sizeof(Berserk::StackAllocator));

    printf("Linked List:     %4li\n", sizeof(Berserk::LinkedList<Berserk::uint64>));


    printf("\n\n");
}

void LinkedListTest()
{
    using namespace Berserk;

    class Element
    {
    public:
        Element(int64 initValue)
        {
            value = initValue;
            printf("Init element with value %li\n", value);
        }

        ~Element()
        {
            printf("Destroy element with value %li\n", value);
        }

    private:
        int64 value;
        char buffer[8];

    };

    LinkedList<Element> TList;
    StaticPoolAllocator MPool;

    MPool.Init(TList.GetSizeOfNode(), 16);
    TList.Init(&MPool);

    MPool.PrintInfo();

    TList.Add(Element(1));
    TList.Add(Element(2));
    TList.Add(Element(3));
    TList.Add(Element(4));
    TList.Add(Element(5));

    TList.Empty();
}

Berserk::CStaticString CStringBuilder(Berserk::uint16 size, char* source)
{
    using namespace Berserk;

    CStaticString target;
    CStaticString another = target;
    target.Init(size, size, 0, source);

    return target;
}

void CStaticStringTest()
{
    using namespace Berserk;

    char hello[] = "Hello, world! It is ASCII string. ";
    CStaticString my_String = CStringBuilder(34, hello);
    printf("Buffer %s\n", my_String.GetCharsBuffer());

    my_String.Copy((CHAR*)"It is fine", 10);
    printf("Copy %s\n", my_String.GetCharsBuffer());

    int32 place = my_String.Find('f');
    printf("Find char %i\n", place);

    my_String.Append((CHAR*)" enough for me", 14);
    printf("Append char* %s\n", my_String.GetCharsBuffer());

    printf("Size %i \n", my_String.GetSize());
    printf("Capacity %i \n", my_String.GetCapacity());

    char another[] = " (may be)";
    CStaticString anotherString = CStringBuilder(9, another);
    printf("Buffer %s\n", anotherString.GetCharsBuffer());

    my_String.Insert(anotherString, 17);
    printf("Insert %s\n", my_String.GetCharsBuffer());

    char mask[] = "Hello, my name is %s, %s, %s...";
    CStaticString stringMask = CStringBuilder(30, mask);
    char name[] = "Egor Orachyov";
    CStaticString stringName = CStringBuilder(13, name);

    my_String.Empty();
    my_String.Copy(stringName, stringMask);
    printf("Copy by mask %s\n", my_String.GetCharsBuffer());


    int32 offset = my_String.Find(stringName);
    printf("Find substring %i\n", offset);

}


#endif //BERSERKENGINE_CLASSTESTING_H
