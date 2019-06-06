#include "CoreSystem/ArrayTest.h"
#include "CoreSystem/LinkedListTest.h"
#include "CoreSystem/FilesTest.h"

int main()
{
    ArrayTest::run();
    LinkedListTest::run();
    FilesTest::run();

    return 0;
}