#include "../Core/Types.h"
#include "../Core/Profiling/MemoryProfiler.h"
#include "../Core/Memory/PoolAllocator.h"
#include "../Core/Memory/StackAllocator.h"
#include "../Core/Cast.h"

using namespace Berserk;

int main() {

    printf("Type:         Size:\n");
    printf("Var: int8      %li \n", sizeof(int8));
    printf("Var: int16     %li \n", sizeof(int16));
    printf("Var: int32     %li \n", sizeof(int32));
    printf("Var: int64     %li \n", sizeof(int64));
    printf("Var: uint8     %li \n", sizeof(uint8));
    printf("Var: uint16    %li \n", sizeof(uint16));
    printf("Var: uint32    %li \n", sizeof(uint32));
    printf("Var: uint64    %li \n", sizeof(uint64));
    printf("Var: float32   %li \n", sizeof(float32));
    printf("Var: float64   %li \n", sizeof(float64));
    printf("Ptr: int32     %li \n", sizeof(int32*));
    printf("Ptr: int64     %li \n", sizeof(int64*));
    printf("Ptr: void      %li \n\n", sizeof(void*));

    PoolAllocator* mAllocator = new PoolAllocator;
    mAllocator->Init(sizeof(int64));

    StackAllocator* mStack = new StackAllocator;
    mStack->Init(sizeof(int64) * 16);

    return 0;
}