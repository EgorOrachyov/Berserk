//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_INHERITANCETESTING_H
#define BERSERKENGINE_INHERITANCETESTING_H

#include <cstdio>

class A
{
public:

    virtual void doA()
    {
        printf("A");
    }
};

class B : public A
{
public:

    virtual void doA() override
    {
        printf("B");
    }

    void doB()
    {
        printf("B");
    }

};

void Test()
{
    A* a = new B();
    a->doA();

    delete a;
}


#endif //BERSERKENGINE_INHERITANCETESTING_H
