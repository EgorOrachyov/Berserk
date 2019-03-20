#include "CoreTest.h"
#include "ThirdPartyTest.h"
#include "FrustumCullingTest.h"
#include "OpenGLTest.h"
#include "OpenGLDriverTest.h"

int main()
{
    /// Core

    // LogTest();
    // OptionTest();
    // AssertTest();
    // AlignmentTest();
    // AllocatorTest();
    XMLTest();
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
    // DynamicStringTest();
    // WCharDynamicStringTest();

    /// ThirdParty

    // RapidXMLTest();

    /// OpenGL

    // BasicOpenGLTest();
    // OpenGLTest();
    // TextureImporterTest();
    OpenGLManagerTest();

    return 0;
}