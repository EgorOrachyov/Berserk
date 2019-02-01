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
    StringUtilityTest();
    StaticStringTest();
    //XMLTest();
    ArrayListTest();
    SharedListTest();
    LinkedListTest();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}