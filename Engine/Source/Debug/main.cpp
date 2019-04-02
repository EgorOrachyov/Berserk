#include "CoreTest.h"
#include "OpenGLDriverTest.h"
#include "EntitySystemTest.h"
#include "RenderingSystemTest.h"

int main()
{
    /// Core

    // LogTest();
    // OptionTest();
    // AssertTest();
    // AlignmentTest();
    // AllocatorTest();
    // ProxyAllocatorTest();
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
    // DynamicStringTest();
    // WCharDynamicStringTest();

    /// Entity System

    BasicClassesTest();

    /// ThirdParty

    // RapidXMLTest();

    /// OpenGL

    // BasicOpenGLTest();
    // OpenGLTest();
    // TextureImporterTest();
    // FrameBufferTest();
    // OpenGLManagerTest();

    /// Render System

    // MaterialImporterTest();

    return 0;
}