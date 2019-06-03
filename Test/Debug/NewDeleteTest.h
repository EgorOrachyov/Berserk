//
// Created by Egor Orachyov on 28.05.2019.
//

#ifndef BERSERK_NEWDELETETEST_H
#define BERSERK_NEWDELETETEST_H

#include <cstdlib>
#include <cstdio>

class Object {

public:

    Object(int a, int b) : ma(a), mb(b)
    {
        printf("Create Object\n");
    }

    virtual ~Object()
    {
        printf("Destroy Object\n");
    }

    void *operator new (size_t size, void* memory)
    {
        printf("Allocate Object %p \n", memory);
        return memory;
    }

    void operator delete (void* memory)
    {
        printf("Delete Object %p \n", memory);
    }

protected:

    int ma;
    int mb;

};

class Component : public Object
{
public:

    Component(int a, int b) : Object(a,b)
    {
        printf("Create Component\n");
    }

    ~Component() override
    {
        printf("Destroy Component\n");
    }

};

void NewDeleteTest()
{
    auto component = new(malloc(sizeof(Component))) Component(1,2);
    delete component;
}


#endif //BERSERK_NEWDELETETEST_H
