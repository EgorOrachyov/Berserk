//
// Created by Egor Orachyov on 01.06.2018.
//

#ifndef BERSERKENGINE_CLASSTESTING_H
#define BERSERKENGINE_CLASSTESTING_H

#include "../Core/Essential/Types.h"

#include "../Core/Memory/StaticPoolAllocator.h"
#include "../Core/Memory/StackAllocator.h"

#include "../Core/Containers/LinkedList.h"

#include "../Core/Strings/StaticStringASCII.h"
#include "../Core/Strings/StaticStringUTF32.h"
#include "../Core/Strings/StringUtils.h"

#include "../Core/Logging/LogManager.h"
#include "../Core/Logging/LogMessages.h"

#include "../Core/Maths/Vectors.h"
#include "../Core/Maths/Matrix4x4.h"

#include <locale.h>

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

/*

Berserk::StaticStringASCII CStringBuilder(Berserk::uint16 size, char* source)
{
    using namespace Berserk;

    StaticStringASCII target;
    StaticStringASCII another = target;
    //target.Init(size, size, 0, source);

    return target;
}

void CStaticStringTest()
{
    using namespace Berserk;

    char hello[] = "Hello, world! It is ASCII string. ";
    StaticStringASCII my_String = CStringBuilder(34, hello);
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
    StaticStringASCII anotherString = CStringBuilder(9, another);
    printf("Buffer %s\n", anotherString.GetCharsBuffer());

    my_String.Insert(anotherString, 17);
    printf("Insert %s\n", my_String.GetCharsBuffer());

    char mask[] = "Hello, my name is %s, %s, %s...";
    StaticStringASCII stringMask = CStringBuilder(30, mask);
    char name[] = "Egor Orachyov";
    StaticStringASCII stringName = CStringBuilder(13, name);

    my_String.Empty();
    my_String.Copy(stringName, stringMask);
    printf("Copy by mask %s\n", my_String.GetCharsBuffer());


    int32 offset = my_String.Find(stringName);
    printf("Find substring %i\n", offset);

}

*/

void StaticStringASCIITestig()
{
    using namespace Berserk;

    StaticStringASCII myName("Egor Orachyov");
    StaticStringASCII whatIsYourName("What is your name?");
    StaticStringASCII finalQuestion;
    StaticStringASCII maskTesting;
    StaticStringASCII systemsNames("Core, Maths, Rendering, Logging, Profiling, Strings");
    StaticStringASCII anotherText(", another params");

    finalQuestion.Copy(whatIsYourName);
    finalQuestion.Append(" [...] ");
    finalQuestion.Append(myName);
    finalQuestion.Append(".");

    printf("%s \n", myName.GetCharsBuffer());
    printf("%s \n", whatIsYourName.GetCharsBuffer());
    printf("%s \n", finalQuestion.GetCharsBuffer());

    printf("Finding 'E' %i \n", finalQuestion.Find('E'));
    printf("Size %i \n", finalQuestion.GetSize());
    printf("Capacity %i \n", finalQuestion.GetCapacity());
    printf("Type %i (=ASCII String) \n", finalQuestion.GetType());

    finalQuestion.Insert(anotherText, 23);
    finalQuestion.Append(".");
    finalQuestion.Append('.');

    printf("%s \n", finalQuestion.GetCharsBuffer());

    maskTesting.Copy("Engine support systems: %s %s ...");
    finalQuestion.Copy(systemsNames, maskTesting);

    printf("%s \n", finalQuestion.GetCharsBuffer());
}

void StaticStringUTF32Testing()
{
    using namespace Berserk;

    // todo: add context initialization and strings settings for
    // todo: correct printing and [... add own print functions and macros]

    setlocale(LC_CTYPE, "");

    StaticStringUTF32 myName(L"Егор Орачев");
    StaticStringUTF32 whatIsYourName(L"What is your name?");
    StaticStringUTF32 finalQuestion;
    StaticStringUTF32 maskTesting;
    StaticStringUTF32 systemsNames(L"Core, Maths, Rendering, Logging, Profiling, Strings");
    StaticStringUTF32 anotherText(L", другие параметры");

    finalQuestion.Copy(whatIsYourName);
    finalQuestion.Append(L" [...] ");
    finalQuestion.Append(myName);
    finalQuestion.Append(L".");

    wprintf(L"%ls \n", myName.GetCharsBuffer());
    wprintf(L"%ls \n", whatIsYourName.GetCharsBuffer());
    wprintf(L"%ls \n", finalQuestion.GetCharsBuffer());

    wprintf(L"Finding 'E' %i \n", finalQuestion.Find('E'));
    wprintf(L"Size %i \n", finalQuestion.GetSize());
    wprintf(L"Capacity %i \n", finalQuestion.GetCapacity());
    wprintf(L"Type %i (=ASCII String) \n", finalQuestion.GetType());

    finalQuestion.Insert(anotherText, 23);
    finalQuestion.Append(L".");
    finalQuestion.Append(L'.');

    wprintf(L"%ls \n", finalQuestion.GetCharsBuffer());

    maskTesting.Copy(L"Engine support systems: %s %s ...");
    finalQuestion.Copy(systemsNames, maskTesting);

    wprintf(L"%ls \n", finalQuestion.GetCharsBuffer());
}

void StringUtilsTesting()
{

}

void VectorsTesting()
{
    using namespace Berserk;

    float32 t = 0;
    Vector3 a = Vector3(2, 5, 7);
    Vector3 b = Vector3(-1, 2, 4);
    Vector3 c = crossProduct(a, b);

    printf("Vec a (%f , %f , %f) \n", a.x, a.y, a.z);
    printf("Vec b (%f , %f , %f) \n", b.x, b.y, b.z);

    printf("Dot product %f \n", dotProduct(a, b));
    printf("Triple product %f \n", tripleProduct(a, b, c));

    printf("Cross product c (%f , %f , %f) \n\n", c.x, c.y, c.z);

    t = 0.1;
    c = lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.5;
    c = lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.7;
    c = lerp(a, b, t);
    printf("Lerp t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.1;
    c = slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.54;
    c = slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.78;
    c = slerp(a, b, t);
    printf("Slerp t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.001;
    c = smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.34;
    c = smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.86;
    c = smoothstep(a, b, t);
    printf("Smoothstep t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);

    t = 0.23;
    c = smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.66;
    c = smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n", t, c.x, c.y, c.z);

    t = 0.91;
    c = smootherstep(a, b, t);
    printf("Smootherstep t=%f vec=(%f , %f , %f) \n\n", t, c.x, c.y, c.z);
}

void MatrixTesting()
{
    using namespace Berserk;

    Matrix4x4 translate = Matrix4x4(1, 0, 0, 3,
                                    0, 1, 0, 5,
                                    0, 0, 1, -4,
                                    0, 0, 0, 1);

    Matrix4x4 scale = Matrix4x4(0.6, 0, 0, 0,
                                0, 0.77, 0, 0,
                                0, 0, 0.23, 0,
                                0, 0, 0, 1);

    Vector4 position = Vector4(1, -5, 3, 1);

    Vector4 real_position = scale * translate * position;

    printf("Real position (%f , %f , %f) \n", real_position.x, real_position.y, real_position.z);
}

#endif //BERSERKENGINE_CLASSTESTING_H
