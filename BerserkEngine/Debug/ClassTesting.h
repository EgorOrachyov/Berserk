//
// Created by Egor Orachyov on 01.06.2018.
//

#ifndef BERSERKENGINE_CLASSTESTING_H
#define BERSERKENGINE_CLASSTESTING_H

#include "../Core/Types.h"

#include "../Core/Memory/PoolAllocator.h"
#include "../Core/Memory/StackAllocator.h"

#include "../Core/Containers/LinkedList.h"

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
    printf("Pool Allocator:  %4li\n", sizeof(Berserk::PoolAllocator));
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
    PoolAllocator MPool;

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


#endif //BERSERKENGINE_CLASSTESTING_H
