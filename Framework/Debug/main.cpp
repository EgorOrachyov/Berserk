#include "ClassTesting.h"
#include "ApplicationContextTesting.h"
#include "../Layout/Debug/LayoutClassTesting.h"
#include "../GLRenderAPI/Debug/GLClassTesting.h"
#include "../GLRenderAPI/Debug/GLSizeTesting.h"

using namespace Berserk;

int main()
{

    /// Core

    //TestClassSize();
    //CRC32Testing();

    //PoolAllocatorTesting();
    //StackAllocatorTesting();
    //DoubleFrameAllocatorTesting();
    //DoubleStackAllocatorTesting();

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
    //MatrixTesting();
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

    ActorProcessTesting();
    ObjectManagerTesting();
    InheritanceTesting();

    /// Application Context

    //RenderSystemTest();
    //ApplicationContextStructureTest();

    return 0;
}