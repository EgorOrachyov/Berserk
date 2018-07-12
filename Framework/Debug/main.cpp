#include "ClassTesting.h"
#include "ApplicationContextTesting.h"
#include "../GLRenderAPI/Debug/GLClassTesting.h"

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

    //CStaticStringTest();
    //StaticStringASCIITestig();
    //StaticStringUTF32Testing();
    //CStringTesting();
    //StringUtils();

    //LogManagerTesting();

    //VectorsTesting();
    //MatrixTesting();
    //QuaternionTesting();

    ConfigTesting();

    /// GL Render API

    //GLGPUProgramTesting();
    //GLDataBufferPackerTesting();
    //GLVertexArrayObjectTesting();
    //GLLightTesting();
    //GLFragmentLightTesting();
    //GLElementBufferTesting();
    //GLToonShadingTesting();
    //GLRenderAPItest();

    /// Application Context

    //RenderSystemTest();
    //ApplicationContextStructureTest();

    return 0;
}