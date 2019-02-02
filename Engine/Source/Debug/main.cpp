#include "CoreTest.h"
#include "ThirdPartyTest.h"

int main()
{
    /// Core

    //LogTest();
    OptionTest();
    AssertTest();
    AlignmentTest();
    AllocatorTest();
    //XMLTest();
    ArrayListTest();
    SharedListTest();
    LinkedListTest();
    StringUtilityTest();
    StaticStringTest();
    DynamicStringTest();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}