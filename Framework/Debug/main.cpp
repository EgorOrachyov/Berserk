#include "ClassTesting.h"
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

    //LinkedListTest();
    //ArrayListTesting();

    //CStaticStringTest();
    //StaticStringASCIITestig();
    //StaticStringUTF32Testing();
    //LogManagerTesting();

    //VectorsTesting();
    //MatrixTesting();
    //QuaternionTesting();

    /// GL Render API

    //GLGPUProgramTesting();
    //GLDataBufferPackerTesting();
    //GLVertexArrayObjectTesting();
    //GLLightTesting();
    GLFragmentLightTesting();
    //GLElementBufferTesting();

    return 0;
}