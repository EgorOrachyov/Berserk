#include "CoreTest.h"
#include "ThirdPartyTest.h"
#include "FrustumCullingTest.h"
#include "Public/GLTexture.h"

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
    // FrustumCullingPerformance();

    /// ThirdParty

    //RapidXMLTest();

    Berserk::GLTexture texture;
    printf("GLTexture mem usage: %u bytes \n", texture.getMemoryUsage());

    return 0;
}