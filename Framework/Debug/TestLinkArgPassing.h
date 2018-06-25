//
// Created by Egor Orachyov on 25.06.2018.
//

#ifndef BERSERKENGINE_TESTLINKARGPASSING_H
#define BERSERKENGINE_TESTLINKARGPASSING_H

#include <cstdio>

class LinkTest
{
public:

    LinkTest(int value)
    {
        this->value = value;
        printf("Construct\n");
    }

    ~LinkTest()
    {
        printf("Destruct\n");
    }

    void doSomething()
    {
        printf("Value %i\n", value);
    }

private:

    int value;
};

void PassSomething(LinkTest& arg)
{
    arg.doSomething();
}

LinkTest& GetSomething()
{
    LinkTest* linkTest = new LinkTest(5); // memory leak
    return *linkTest;
}

void Test()
{
    LinkTest linkTest(10);

    PassSomething(linkTest);

    LinkTest& link = GetSomething();

    link.doSomething();
}

#endif //BERSERKENGINE_TESTLINKARGPASSING_H
