#include "CoreTest.h"
#include "ThirdPartyTest.h"

int main()
{
    /// Core

    LogTest();
    OptionTest();
    AssertTest();
    AlignmentTest();
    AllocatorTest();
    StringUtilityTest();
    StaticStringTest();
    ArrayListTest();
    SharedListTest();

    //XMLTest();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}