#include "../Core/Types.h"
#include "../Core/Profiling/MemoryProfiler.h"
#include "../Core/Memory/PoolAllocator.h"
#include "../Core/Memory/StackAllocator.h"
#include "../Core/Containers/LinkedList.h"
#include "ClassTesting.h"

using namespace Berserk;

int main() {

    TestClassSize();

    PoolAllocator* mAllocator = new PoolAllocator;
    mAllocator->Init(sizeof(int64));

    StackAllocator* mStack = new StackAllocator;
    mStack->Init(sizeof(int64) * 16);

    return 0;
}