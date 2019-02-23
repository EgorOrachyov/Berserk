#include "CoreTest.h"
#include "ThirdPartyTest.h"
#include "FrustumCullingTest.h"
#include "OpenGLTest.h"

int main()
{
    /// Core

    // LogTest();
    // OptionTest();
    // AssertTest();
    // AlignmentTest();
    // AllocatorTest();
    // XMLTest();
    // StringUtilityTest();
    // StaticStringTest();
    // DynamicStringTest();
    // ArrayListTest();
    // SharedListTest();
    // LinkedListTest();
    // HashMapTest();
    // MathTest();
    // GeometryTest();
    // SIMDTest();
    // FrustumTest();
    // TransformTest();
    // ThreadTest();
    // ThreadPoolTest();
    // FrustumCullingPerformance();
    // OperatorTest();

    /// ThirdParty

    // RapidXMLTest();

    /// OpenGL

    // BasicOpenGLTest();
    OpenGLDriverTest();

    return 0;
}