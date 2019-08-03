//
// Created by Egor Orachyov on 2019-08-02.
//

#ifndef BERSERK_OBJECTSTEST_H
#define BERSERK_OBJECTSTEST_H

#include <Object/Object.h>
#include <Resource/ResourceHandle.h>

using namespace Berserk;

class ObjectsTest
{
public:

    static void ObjectBaseTest1()
    {
        IAllocator& allocator = Allocator::get();
        auto object = allocator.engnie_new<Object>(0);

        OutputDevice::printf("Run-time type name: %s, type id: %u \n",
                object->getType().get(), object->getTypeId());

        allocator.engine_destroy(object);
    }

    static void run()
    {
        ObjectBaseTest1();
    }

};


#endif //BERSERK_OBJECTSTEST_H
