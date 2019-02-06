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
    StringUtilityTest();
    StaticStringTest();
    DynamicStringTest();
    ArrayListTest();
    SharedListTest();
    LinkedListTest();
    HashMapTest();
    MathTest();
    GeometryTest();
    SIMDTest();
    FrustumTest();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}