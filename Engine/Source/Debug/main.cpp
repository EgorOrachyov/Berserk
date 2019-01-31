#include "CoreTest.h"
#include "ThirdPartyTest.h"

int main()
{
    /// Core

    OptionTest();
    AssertTest();
    AlignmentTest();
    AllocatorTest();
    StringUtilityTest();
    StaticStringTest();

    /// ThirdParty

    RapidXMLTest();

    return 0;
}