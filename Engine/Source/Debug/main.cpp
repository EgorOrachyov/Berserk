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

    //XMLTest();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}