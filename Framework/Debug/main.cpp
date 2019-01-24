// #include "ClassTesting.h"
// #include "ApplicationContextTesting.h"
// #include "NewFragmentLightningTesting.h"
// #include "Debug/LayoutClassTesting.h"
// #include "../GLRenderAPI/Debug/GLClassTesting.h"
// #include "../GLRenderAPI/Debug/GLSizeTesting.h"
// #include "FileReadingTesting.h"
#include "Public/Test.h"

int main()
{

    /// New Core

    AssertTest();
    AlignmentTest();
    AllocatorTest();
    PoolAllocatorTest();

    /// Core

    //TestClassSize();
    //CRC32Testing();

    //PoolAllocatorTesting();
    //StackAllocatorTesting();
    //DoubleFrameAllocatorTesting();
    //DoubleStackAllocatorTesting();
    //ListAllocatorTesting();
    //ListAllocatorBuffersTesting();

    //LinkedListTest();
    //ArrayListTesting();
    //HashTableTesting();
    //DoubleLinkedListTest();
    //QueueTesting();

    //CStaticStringTest();
    //StaticStringASCIITestig();
    //StaticStringUTF32Testing();
    //CStringTesting();
    //StringUtils();

    //LogManagerTesting();

    //VectorsTesting();
    // -- MatrixTesting();
    //QuaternionTesting();

    //ConfigTesting();
    //ArrayListPinterTest();

    /// GL Render API

    //GLClassSizeTesting();

    //GLGPUProgramTesting();
    //GLDataBufferPackerTesting();
    //GLVertexArrayObjectTesting();
    //GLLightTesting();
    //GLFragmentLightTesting();
    //GLElementBufferTesting();
    //GLToonShadingTesting();
    //GLRenderAPItest();

    /// Layout

    //ActorProcessTesting();
    //ObjectManagerTesting();
    //InheritanceTesting();

    /// Application Context

    //RenderSystemTest();
    //ApplicationContextStructureTest();
    //myAppTesting();
    //NewFragLightTesting();

    /// Additional
    //FileReadingTest();

    return 0;
}