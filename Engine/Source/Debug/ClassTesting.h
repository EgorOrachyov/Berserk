//
// Created by Egor Orachyov on 01.06.2018.
//

#ifndef BERSERKENGINE_CLASSTESTING_H
#define BERSERKENGINE_CLASSTESTING_H

#include "Misc/Types.h"
#include "HashFunctions/CRC32.h"

#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "Memory/DoubleStackAllocator.h"
#include "Memory/DoubleFrameAllocator.h"
#include "Memory/ListAllocator.h"

#include "Containers/SharedList.h"
#include "Containers/LinkedList.h"
#include "Containers/ArrayList.h"
#include "Containers/HashTable.h"
#include "Containers/DoubleLinkedList.h"
#include "Containers/List.h"

#include "Strings/CStaticString.h"
#include "Strings/WStaticString.h"
#include "Strings/CString.h"
#include "Strings/WString.h"

#include "Strings/UtilityString.h"

#include "Logging/LogManager.h"
#include "Logging/LogMessages.h"

#include "Math/MathInclude.h"
#include "Math/MathUtility.h"
#include "Math/UtilityMatrices.h"
#include "Math/UtilityQuaternions.h"

#include "Config/ConfigTable.h"
#include "Config/ConfigLoader.h"
#include "Misc/Buffers.h"

#include <locale.h>

void TestVariableSize()
{
    printf("Type:         Size:\n");
    printf("Var: INT8      %li \n", sizeof(Berserk::INT8));
    printf("Var: INT16     %li \n", sizeof(Berserk::INT16));
    printf("Var: INT32     %li \n", sizeof(Berserk::INT32));
    printf("Var: INT64     %li \n", sizeof(Berserk::INT64));
    printf("Var: UINT8     %li \n", sizeof(Berserk::UINT8));
    printf("Var: UINT16    %li \n", sizeof(Berserk::UINT16));
    printf("Var: UINT32    %li \n", sizeof(Berserk::UINT32));
    printf("Var: UINT64    %li \n", sizeof(Berserk::UINT64));
    printf("Var: FLOAT32   %li \n", sizeof(Berserk::FLOAT32));
    printf("Var: FLOAT64   %li \n", sizeof(Berserk::FLOAT64));
    printf("Ptr: INT32     %li \n", sizeof(Berserk::INT32*));
    printf("Ptr: INT64     %li \n", sizeof(Berserk::INT64*));

    printf("\n\n");
}

void TestClassSize()
{
    printf("Pool Allocator:  %4li\n", sizeof(Berserk::PoolAllocator));
    printf("Stack Allocator: %4li\n", sizeof(Berserk::StackAllocator));

    printf("Linked List:     %4li\n", sizeof(Berserk::SharedList<Berserk::UINT64>));


    printf("\n\n");
}

void LogManagerTesting()
{
    using namespace Berserk;

    //LogManager& logManager = LogManager::GetGlobalLogManager();
    //logManager.PushMessage(LogMessageType::LMT_INFO, "Test message");

    INFO("This is test error: %i, %i", 1, 2);
    WARNING("This is test error: %i, %i", 1, 2);
    ERROR("This is test error: %i, %i", 1, 2);

    PUSH("Test push messaging macro: %i ~ ok", 1);

    OPEN_BLOCK("Testing of block messages");
    PUSH_BLOCK("This is the string num %i", 1);
    PUSH_BLOCK("This is the string num %i", 1);
    PUSH_BLOCK("This is the string num %i", 1);
    CLOSE_BLOCK();
}

void LinkedListTest()
{
    using namespace Berserk;

    struct Element
    {
    public:
        Element(long initValue)
        {
            value = initValue;
            printf("Init element with value %li\n", value);
        }

        ~Element()
        {
            printf("Destroy element with value %li\n", value);
        }

    public:

        INT64 value;
        char buffer[8];

    };

    LinkedList<Element> TList;
    TList.init();

    Element e = Element(1);
    TList.add(e);
    e = Element(2);
    TList.add(e);
    e = Element(3);
    TList.add(e);

    TList.iterate(true);
    while (TList.iterate())
    {
        printf("%li\n", TList.getCurrent().value);
    }

    TList.empty();
}

void ArrayListTesting()
{
    using namespace Berserk;

    struct Node
    {
    public:
        Node(float v)
        {
            value = v;
            printf("Create node %f\n", value);
        }
        ~Node()
        {
            printf("Destroy node %f\n", value);
        }

    public:

        float value;

    };

    ArrayList<Node> list;

    list.add(Node(1));
    list.add(Node(2));
    list.add(Node(3));
    list.add(Node(4));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.add(Node(5));
    list.add(Node(6));
    list.add(Node(7));
    list.add(Node(8));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.add(Node(1));
    list.add(Node(2));
    list.add(Node(3));
    list.add(Node(4));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.add(Node(5));
    list.add(Node(6));
    list.add(Node(7));
    list.add(Node(8));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.add(Node(1));
    list.add(Node(2));
    list.add(Node(3));
    list.add(Node(4));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.add(Node(5));
    list.add(Node(6));
    list.add(Node(7));
    list.add(Node(8));

    printf("Capacity %i Size %i\n", list.getCapacity(), list.getSize());

    list.remove(0);
    list.add(Node(100));

    for(UINT32 i = 0; i < list.getSize(); i++)
    {
        printf("%f\n", list.get(i).value);
    }

}

void QueueTesting()
{
    using namespace Berserk;

    List<UINT32> queue;
    queue.init(8);
    queue.lock();

    queue.add(1);
    queue.add(2);
    queue.add(3);
    queue.add(4);
    queue.add(5);
    queue.add(6);
    queue.add(7);
    queue.add(8);
    queue.add(9);
    queue.add(10);

    for(UINT32 i = 0; i < queue.getSize(); i++)
    {
        PUSH("Q[%u] = %u", i, queue.get(i));
    }

    queue.clean();
    queue.unlock();

    queue.add(1);
    queue.add(2);
    queue.add(3);
    queue.add(4);
    queue.add(5);
    queue.add(6);
    queue.add(7);
    queue.add(8);
    queue.add(9);
    queue.add(10);

    for(UINT32 i = 0; i < queue.getSize(); i++)
    {
        PUSH("Q[%u] = %u", i, queue.get(i));
    }
}

void DoubleLinkedListTest()
{
    using namespace Berserk;

    struct Node
    {
    public:
        Node(float v)
        {
            value = v;
            printf("Create node %f\n", value);
        }
        ~Node()
        {
            printf("Destroy node %f\n", value);
        }

        const bool operator == (const Node& another)
        {
            return (value == another.value);
        }

    public:

        float value;

    };

    DoubleLinkedList<Node> list;
    list.init();

    list.add(Node(1));
    list.add(Node(2));
    list.add(Node(3));
    list.add(Node(4));
    list.add(Node(5));
    list.add(Node(6));

    printf("Size %i\n", list.getSize());

    list.removeFirst();
    list.removeLast();

    list.iterate(true);
    while (list.iterate())
    {
        printf("Value %f\n", list.getCurrent().value);
    }

    printf("Size %i\n", list.getSize());

    list.remove(Node(1));
    list.remove(Node(2));

    list.iterate(true);
    while (list.iterate())
    {
        printf("Value %f\n", list.getCurrent().value);
    }

    printf("Size %i\n", list.getSize());

}

void StackAllocatorTesting()
{
    using namespace Berserk;

    StackAllocator allocator;
    allocator.init(1024);

    for (UINT32 i = 1; i < 100; i++)
    {
        printf("Alloc %p \n", allocator.alloc(i));
    }
}

void DoubleStackAllocatorTesting()
{
    using namespace Berserk;

    DoubleStackAllocator allocator;
    allocator.init(1024);

    for (UINT32 i = 1; i < 100; i++)
    {
        printf("Alloc head %p ", allocator.allocHead(i));
        printf("Alloc tail %p\n", allocator.allocTail(i));

    }
}

/*

Berserk::CHARStaticString CStringBuilder(Berserk::UINT16 size, char* source)
{
    using namespace Berserk;

    CHARStaticString target;
    CHARStaticString another = target;
    //target.Init(size, size, 0, source);

    return target;
}

void CStaticStringTest()
{
    using namespace Berserk;

    char hello[] = "Hello, world! It is ASCII string. ";
    CHARStaticString my_String = CStringBuilder(34, hello);
    printf("Buffer %s\n", my_String.GetCharsBuffer());

    my_String.Copy((CHAR*)"It is fine", 10);
    printf("Copy %s\n", my_String.GetCharsBuffer());

    INT32 place = my_String.Find('f');
    printf("Find char %i\n", place);

    my_String.Append((CHAR*)" enough for me", 14);
    printf("Append char* %s\n", my_String.GetCharsBuffer());

    printf("Size %i \n", my_String.GetSize());
    printf("Capacity %i \n", my_String.GetCapacity());

    char another[] = " (may be)";
    CHARStaticString anotherString = CStringBuilder(9, another);
    printf("Buffer %s\n", anotherString.GetCharsBuffer());

    my_String.Insert(anotherString, 17);
    printf("Insert %s\n", my_String.GetCharsBuffer());

    char mask[] = "Hello, my name is %s, %s, %s...";
    CHARStaticString stringMask = CStringBuilder(30, mask);
    char name[] = "Egor Orachyov";
    CHARStaticString stringName = CStringBuilder(13, name);

    my_String.Empty();
    my_String.Copy(stringName, stringMask);
    printf("Copy by mask %s\n", my_String.GetCharsBuffer());


    INT32 offset = my_String.Find(stringName);
    printf("Find substring %i\n", offset);

}

*/

void StaticStringASCIITestig()
{
    using namespace Berserk;

    CStaticString myName("Egor Orachyov");
    CStaticString whatIsYourName("What is your name?");
    CStaticString finalQuestion;
    CStaticString maskTesting;
    CStaticString systemsNames("Core, Maths, Rendering, Logging, Profiling, Strings");
    CStaticString anotherText(", another params");
    CStaticString staticString = "Explicit initialization is needed";

    finalQuestion = CStaticString("Lol");
    finalQuestion.copy(whatIsYourName);
    finalQuestion.append(" [...] ");
    finalQuestion.append(myName);
    finalQuestion.append(".");

    printf("%s \n", staticString.getChars());
    printf("%s \n", myName.getChars());
    printf("%s \n", whatIsYourName.getChars());
    printf("%s \n", finalQuestion.getChars());

    printf("Finding 'E' %i \n", finalQuestion.find('E'));
    printf("Size %i \n", finalQuestion.getSize());
    printf("Capacity %i \n", finalQuestion.getCapacity());
    printf("Type %i (=ASCII String) \n", finalQuestion.getType());

    finalQuestion.insert(anotherText, 23);
    finalQuestion.append(".");
    finalQuestion.append('.');

    printf("%s \n", finalQuestion.getChars());

    maskTesting.copy("Engine support systems: %s %s ...");
    finalQuestion.copy(systemsNames, maskTesting);

    printf("%s \n", finalQuestion.getChars());

    CStaticString berserkEngine = CStaticString("Berserk Engine");
    CStaticString afterPlus;
    CStaticString toAppend = CStaticString(" is not ready to work!");
    afterPlus = berserkEngine + toAppend;
    printf("After = and + %s", afterPlus.getChars());
}

void StaticStringUTF32Testing()
{
    using namespace Berserk;

    // todo: add context initialization and strings settings for
    // todo: correct printing and [... add special print functions and macros]

    setlocale(LC_CTYPE, "");

    WStaticString myName(L"Егор Орачев");
    WStaticString whatIsYourName(L"What is your name?");
    WStaticString finalQuestion;
    WStaticString maskTesting;
    WStaticString systemsNames(L"Core, Maths, Rendering, Logging, Profiling, Strings");
    WStaticString anotherText(L", другие параметры");

    finalQuestion.copy(whatIsYourName);
    finalQuestion.append(L" [...] ");
    finalQuestion.append(myName);
    finalQuestion.append(L".");

    wprintf(L"%ls \n", myName.getChars());
    wprintf(L"%ls \n", whatIsYourName.getChars());
    wprintf(L"%ls \n", finalQuestion.getChars());

    wprintf(L"Finding 'E' %i \n", finalQuestion.find(L'E'));
    wprintf(L"Size %i \n", finalQuestion.getSize());
    wprintf(L"Capacity %i \n", finalQuestion.getCapacity());
    wprintf(L"Type %i (=ASCII String) \n", finalQuestion.getType());

    finalQuestion.insert(anotherText, 23);
    finalQuestion.append(L".");
    finalQuestion.append(L'.');

    wprintf(L"%ls \n", finalQuestion.getChars());

    maskTesting.copy(L"Engine support systems: %s %s ...");
    finalQuestion.copy(systemsNames, maskTesting);

    wprintf(L"%ls \n", finalQuestion.getChars());
}

void StringUtilsTesting()
{

}

void VectorsTesting()
{
    using namespace Berserk;

    FLOAT32 t = 0;
    Vector3f a = Vector3f(2.0f, 5.0f, 17.0f);
    Vector3f b = Vector3f(-1.0f, 12.0f, 4.0f);
    Vector3f c = Vector3f::cross(a, b);

    printf("Vec a (%f , %f , %f) \n", a.x, a.y, a.z);
    printf("Vec b (%f , %f , %f) \n", b.x, b.y, b.z);

    printf("Dot product %f \n", Vector3f::dot(a, b));
    printf("Triple product %f \n", Vector3f::triple(a, b, c));

    printf("Cross product c (%f , %f , %f) \n\n", c.x, c.y, c.z);

    t = 0.1;
    c = Vector3f::lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.5;
    c = Vector3f::lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.7;
    c = Vector3f::lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.1;
    c = Vector3f::slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.54;
    c = Vector3f::slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.78;
    c = Vector3f::slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.001;
    c = Vector3f::smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.34;
    c = Vector3f::smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.86;
    c = Vector3f::smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.23;
    c = Vector3f::smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.66;
    c = Vector3f::smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.91;
    c = Vector3f::smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    printf("To String Vector test:\n");
    printf("%s \n", Vector2f(0.23f, -1.603f).toString().getChars());
    printf("%s \n", Vector3f(0.4f, 0.8f, 0.33f).toString().getChars());
    printf("%s \n", Vector4f(0.8f, -0.77f, 3.6f, -1.01f).toString().getChars());
}

void MatrixTesting()
{
    using namespace Berserk;

    Matrix4x4f translate = Matrix4x4f(1, 0, 0, 3,
                                    0, 1, 0, 5,
                                    0, 0, 1, -4,
                                    0, 0, 0, 1);

    Matrix4x4f scale = Matrix4x4f(0.6, 0, 0, 0,
                                0, 0.77, 0, 0,
                                0, 0, 0.23, 0,
                                0, 0, 0, 1);

    Vector4f position = Vector4f(1, -5, 3, 1);

    Vector4f real_position = scale * translate * position;

    printf("Real position (%f , %f , %f) \n", real_position.x, real_position.y, real_position.z);

    // Transformations

    Matrix4x4f look = lookAt(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    Vector4f point = Vector4f(0, 1, 0, 1);

    Vector4f result = look * point;

    printf("Real position (%f , %f , %f) \n", result.x, result.y, result.z);

    // Projection

    Matrix4x4f project = perspective(Math::radians(45.0f), 1, 0.1, 100);
    result = project * look * point;

    printf("Real position (%f , %f , %f) \n", result.x / result.w, result.y / result.w, result.z / result.w);

    project = orthographic(0, 1000, 0, 800, 0, 100);
    point = Vector4f(267, 456, -77, 1);
    result = project * point;

    printf("Real position (%f , %f , %f) \n\n", result.x / result.w, result.y / result.w, result.z / result.w);

    Matrix4x4f P = perspective(Math::radians(120.0f), 1, 0.1, 10);
    Vector4f v = Vector4f(1.0f, 1.0f, -5.0f, 1.0f);
    Vector4f p = Vector4f(0.0f, 0.0f, -5.0f, 1.0f);

    printf("Project v: %s , p: %s \n", ((P * v) / (P * v).w).toString().getChars(), ((P * p) / (P * p).w).toString().getChars());

}

void QuaternionTesting()
{
    using namespace Berserk;

    Vector3f axis = Vector3f(1,0,0);
    FLOAT32 angle = 90;
    Quatf quat = fromVector(axis, Math::radians(angle));
    Vector3f point = Vector3f(1, 1, 0);
    Vector3f res = rotate(quat, point);

    printf("Rotate vec=(%f;%f;%f) by quat=(%f;%f;%f;%f) on angle=(%f deg) res=(%f;%f;%f)\n",
           point.x, point.y, point.z,
           quat.s, quat.x, quat.y, quat.z,
           angle,
           res.x, res.y, res.z);

    quat = fromVector(Vector3f(1,1,1), Math::radians(33.0f));
    Quatf quatFromMat = fromMatrix(fromQuaternion(quat));

    printf("Before quat=(%f;%f;%f;%f) after quat=(%f;%f;%f;%f)\n",
           quat.s, quat.x, quat.y, quat.z,
           quatFromMat.s, quatFromMat.x, quatFromMat.y, quatFromMat.z);
}

void CRC32Testing()
{
    using namespace Berserk;

    printf("CRC32 INT32 testing\n");
    for(int i = 0; i < 100; i++)
    {
        printf("Value = %10i Hash = %10u \n", i, hashCRC32(i));
    }

    printf("\n");

    printf("CRC32 INT64 testing\n");
    for(FLOAT32 i = 0; i < 100; i += 1.0f)
    {
        printf("Value = %10f Hash = %10u \n", i, hashCRC32(i));
    }

    printf("\n");

    printf("Value = %s Hash = %10u \n", "Hello world", hashCRC32("Hello world", 11));
    printf("Value = %s Hash = %10u \n", "Another message", hashCRC32("Another message", 15));


}

void PoolAllocatorTesting()
{
    using namespace Berserk;

    PoolAllocator pool;
    pool.init(17, 30);

    pool.printInfo();

    for (int i = 0; i < 150; i++)
    {
        printf("[%3i] %p\n", i, pool.allocBlock());

        if (i == 44) pool.setOneBufferCapacity(60);
        if (i == 74) pool.setStatic(true);
    }
}

void DoubleFrameAllocatorTesting()
{
    using namespace Berserk;

    DoubleFrameAllocator dfa;
    dfa.init(Buffers::SIZE_1024);

    printf("First stack\n");

    for(UINT32 i = 1; i < 100; i++ )
    {
        printf("Alloc: %p \n", dfa.allocBlock(16));
    }

    dfa.swap();

    printf("Second stack\n");

    for(UINT32 i = 1; i < 100; i++ )
    {
        printf("Alloc: %p \n", dfa.allocBlock(16));
    }

}

void ListAllocatorTesting()
{
    using namespace Berserk;

    ListAllocator allocator;

    allocator.init();

    UINT32 size = 32;
    CHAR* data = (CHAR*)allocator.alloc(size);
    strcpy(data, "Something");
    printf("Data %s \n", data);

    const UINT32 count = 10;
    INT64* array[count];

    printf("List Allocator Info: buffer size %u, allocated memory %u, block size %u, buffer %p \n",
           allocator.getBufferSize(),
           allocator.getAllocatedMemory(),
           allocator.getMinimalBlockSize(),
           allocator.getBuffer());

    allocator.free(data);

    printf("List Allocator Info: buffer size %u, allocated memory %u, block size %u, buffer %p \n",
           allocator.getBufferSize(),
           allocator.getAllocatedMemory(),
           allocator.getMinimalBlockSize(),
           allocator.getBuffer());

    for(UINT32 i = 0; i < count; i++)
    {
        array[i] = allocator.alloc<INT64>();
    }

    allocator.printBlocksInfo();

    for(UINT32 i = 0; i < count; i++)
    {
        allocator.free<INT64>(array[i]);
    }

    allocator.printBlocksInfo();

    struct Element
    {
    public:

        ~Element()
        { printf("Delete element %p | %li %li %lf \n", this, data1, data2, data3); }

        void init(INT64 a, INT64 b = 1, INT64 c = 2)
        { data1 = a; data2 = a + a * a; data3 = (FLOAT64)(data2) / (FLOAT64)(2 + data2 * data2); }

        INT64 data1;
        INT64 data2;
        FLOAT64 data3;
    };

    const UINT32 elementsCount = 64;
    const UINT32 halfPart = 17;
    Element* elements[elementsCount];

    for(UINT32 i = 0; i < elementsCount; i++)
    {
        elements[i] = allocator.alloc<Element>();
        elements[i]->init(i);
    }

    allocator.printBlocksInfo();

    for(UINT32 i = 0; i < halfPart; i++)
    {
        allocator.free<Element>(elements[i]);
    }

    allocator.printBlocksInfo();

    PUSH("Unite allocator blocks");

    allocator.printBlocksInfo();

    for(UINT32 i = halfPart; i < elementsCount; i++)
    {
        allocator.free<Element>(elements[i]);
    }

    allocator.printBlocksInfo();

    PUSH("Unite allocator blocks");

    allocator.printBlocksInfo();

    allocator.printBuffersInfo();

    for(UINT32 i = 0; i < elementsCount; i++)
    {
        elements[i] = allocator.alloc<Element>();
        elements[i]->init(i);
    }

    allocator.printBlocksInfo();

    for(UINT32 i = halfPart; i < elementsCount; i++)
    {
        allocator.free<Element>(elements[i]);
    }

    allocator.printBlocksInfo();
    allocator.reset();
}

void ListAllocatorBuffersTesting()
{
    using namespace Berserk;

    struct HeavyObject
    {
    public:

        ~HeavyObject() { /* printf("HO p = %p i = %i \n", this, index); */ };
        void init(UINT32 i) { index = i; value1 = i * i * i; value2 = (10 + value1 * value1) / (value1 + 1); }

        UINT32 index;
        FLOAT32 value1;
        FLOAT32 value2;
        CHAR array[100];
    };

    ListAllocator alloc;

    alloc.init();
    alloc.printBlocksInfo();

    const UINT32 COUNT = 100;
    HeavyObject* object[COUNT];

    PUSH("Alloc %u objects", COUNT);
    for(UINT32 i = 0; i < COUNT; i++)
    {
        object[i] = alloc.alloc<HeavyObject>();
        object[i]->init(i);
    }

    alloc.printBlocksInfo();

    PUSH("Free %u objects", COUNT);
    for(UINT32 i = 0; i < COUNT; i++)
    {
        alloc.free<HeavyObject>(object[i]);
    }

    alloc.printBlocksInfo();

    alloc.printBuffersInfo();
    alloc.reset();
}

void HashTableTesting()
{
    using namespace Berserk;

    HashTable<INT32, INT64> table;
    table.init(16);

    for(INT32 i = 0; i < 40; i++)
    {
        table.add(i, i << 3);
    }

    printf("Size %u \n", table.getSize());
    printf("Range %u \n", table.getRange());
    printf("Capacity %u \n", table.getCapacity());
    printf("Load factor %f \n", table.getLoadFactor());

    for(INT32 i = 0; i < 10; i++)
    {
        table.remove(i * 2);
    }

    printf("Size %u \n", table.getSize());
    printf("Range %u \n", table.getRange());
    printf("Capacity %u \n", table.getCapacity());
    printf("Load factor %f \n", table.getLoadFactor());


    HashTable<UINT32, CStaticString> hashTable;
    hashTable.init(10);


    CStaticString myName = CStaticString("Egor");
    UINT32 myNameID = hashCRC32(myName.getChars(), myName.getSize());

    CStaticString mySurname = CStaticString("Orachyov");
    UINT32 mySurnameID = hashCRC32(mySurname.getChars(), myName.getSize());

    hashTable.add(myNameID, myName);
    hashTable.add(mySurnameID, mySurname);

    printf("Get string: %s via id %u \n", hashTable.get(myNameID).getChars(), myNameID);
    printf("Get string: %s via id %u \n", hashTable.get(mySurnameID).getChars(), mySurnameID);

}

void CStringTesting()
{
    using namespace Berserk;

    CString myName = CString("Some name of a person");
    CString anotherName = CString(10, "0123456789");
    CString bigString = CString("If you want to store really big string in the engine program, you can use this ascii string, "
                                        "which can store nearly 1024 symbols in one buffer. And it will be done with proper "
                                        "use of dynamic engine memory. More over, you do not have to use new operator, all strings "
                                        "will be stored in the stack and automatically destroyed.");


    CString s1 = CString("String 1, ");
    CString s2 = CString("String 2, ");
    CString s3 = CString("String 3, ");
    CString s4 = CString("String 4, ");

    CString s = CString("Test ");

    s += s1; s += s2; s += s3; s += s4;
    printf("S = s1 + s2 + s3 + s4 : %s\n", s.getChars());

    s += bigString;
    printf("S = s + bigString : %s\n", s.getChars());

    CStaticString st_str = CStaticString("Label 1");
    CString str_interract_with = CString("This + static string: ");
    str_interract_with += st_str;

    printf("Dynamic + static %s\n", str_interract_with.getChars());
    printf("Are equal %i \n", st_str == st_str && str_interract_with == str_interract_with);

    CString toCopy = CString("Old");
    CString source = CString("Something new");
    toCopy = source;

    printf("Assign operation %s\n", toCopy.getChars());

    CStaticString empty;
    CStaticString content = CStaticString("New name for someone");
    empty = empty + content;
    printf("Content %s\n", empty.getChars());
}

void StringUtils()
{
    using namespace Berserk;

    CText ctext = CTEXT("Macro to use dynamic strings");
    WText wtext = WTEXT("Макрос для использования длинных строк");

    CName cname = CNAME("Test static c string");
    WName wname = WNAME("Макрос для статических длинных строк");

    // todo: add context initialization and strings settings for
    // todo: correct printing and [... add special print functions and macros]

    //setlocale(LC_CTYPE, "");

    printf("CName %s\n", cname.getChars());
    wprintf(L"WName %ls\n", wname.getChars());

    printf("CText %s\n", ctext.getChars());
    wprintf(L"WText %ls\n", wtext.getChars());

    WCHAR buffer[Buffers::SIZE_64];
    buffer[0] = L'\0';
    WWRITE(buffer, "Русское послание: %ls\n", wname.getChars());

    wprintf(L"%ls", buffer);


}

void ConfigTesting()
{
    using namespace Berserk;

    CName name1 = CNAME("Label1");
    CName name2 = CNAME("Label1");

    printf("Hash buffer %i %i \n",
           hashCRC32(name1.getChars(), name1.getSize()),
           hashCRC32(name2.getChars(), name2.getSize()));

    printf("Hash object %i %i \n",
           hashCRC32((CHAR *)&name1, sizeof(name1)),
           hashCRC32((CHAR *)&name2, sizeof(name2)));

    ConfigTable table;
    table.init(10);

    bool isLoad = loadConfigList("../Core/Config/ConfigList.cfg", table);

    if (isLoad)
    {
        printf("Window name: %s size: %u x %u \n",
               (CHAR*)table.get(CStaticString("ApplicationName")),
               *(UINT32*)table.get(CStaticString("WindowWidth")),
               *(UINT32*)table.get(CStaticString("WindowHeight")));

        printf("Window name: %s size: %u x %u \n",
               (CHAR*)table.get("ApplicationName"),
               *(UINT32*)table.get("WindowWidth"),
               *(UINT32*)table.get("WindowHeight"));
    }


    isLoad = loadConfigFile("../Core/Config/engine.cfg", table);

    if (isLoad)
    {
        printf("Cfg Window name: %s size: %u x %u \n",
               (CHAR*)table.get(CStaticString("ApplicationName")),
               *(UINT32*)table.get(CStaticString("WindowWidth")),
               *(UINT32*)table.get(CStaticString("WindowHeight")));

        printf("Cfg Window name: %s size: %u x %u \n",
               (CHAR*)table.get("ApplicationName"),
               *(UINT32*)table.get("WindowWidth"),
               *(UINT32*)table.get("WindowHeight"));

        printf("\nGet values via new functions\n");

        printf("Window name: %s size: %u x %u \n",
               table.getChar("ApplicationName"),
               table.getUInt32("WindowWidth"),
               table.getUInt32("WindowHeight"));

        printf("Render System: %s \n", table.getChar("RenderSystem"));
    }

}

void ArrayListPinterTest()
{
    using namespace Berserk;

    struct Node
    {
    public:

        Node(UINT32 num) { data = num; }
        ~Node() {printf("Destroy %u \n", data); }

        UINT32 data;
    };

    ArrayList<Node*> list;
    list.init();

    Node node = 10;

    list.add(&node);
    list.add(&node);
    list.add(&node);
    list.add(&node);

    for(UINT32 i = 0; i < list.getSize(); i++)
        printf("list[%u] = %u \n", i, list.get(i)->data);

}

#endif //BERSERKENGINE_CLASSTESTING_H
