#include "CoreTest.h"
#include "ThirdPartyTest.h"
#include "FrustumCullingTest.h"

int main()
{
    /// Core

    // LogTest();
    OptionTest();
    AssertTest();
    AlignmentTest();
    AllocatorTest();
    // XMLTest();
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
    TransformTest();
    // ThreadTest();
    // ThreadPoolTest();
    FrustumCullingPerformance();

    /// ThirdParty

    //RapidXMLTest();

    return 0;
}