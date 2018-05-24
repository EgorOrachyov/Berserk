#include "../Core/Types.h"
#include "../Core/Assert.h"

using namespace Berserk;

int main() {

    printf("Type:         Size:\n");
    printf("Var: int8      %li \n", sizeof(int8));
    printf("Var: int16     %li \n", sizeof(int16));
    printf("Var: int32     %li \n", sizeof(int32));
    printf("Var: int62     %li \n", sizeof(int62));
    printf("Var: uint8     %li \n", sizeof(uint8));
    printf("Var: uint16    %li \n", sizeof(uint16));
    printf("Var: uint32    %li \n", sizeof(uint32));
    printf("Var: uint62    %li \n", sizeof(uint62));
    printf("Var: float32   %li \n", sizeof(float32));
    printf("Var: float64   %li \n", sizeof(float64));

    return 0;
}