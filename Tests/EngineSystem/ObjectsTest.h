//
// Created by Egor Orachyov on 2019-08-02.
//

#ifndef BERSERK_OBJECTSTEST_H
#define BERSERK_OBJECTSTEST_H

#include <Object/Object.h>
#include <Resource/ResourceHandle.h>
#include <Components/CameraComponent.h>

using namespace Berserk;

class ObjectsTest
{
public:

    static void ObjectBaseTest1()
    {
        IAllocator& allocator = Allocator::get();
        auto object = allocator.engine_new_no_args<Object>();

        OutputDevice::printf("Run-time type name: %s, type id: %u \n",
                object->getType().get(), object->getTypeId());

        allocator.engine_destroy(object);

    }

    static void ObjectBaseTest2()
    {
        IAllocator& allocator = Allocator::get();

        auto _array = allocator.engnie_new<TArray<String>>(allocator);
        TSharedPtr<TArray<String>> array(_array, &allocator);

        array->emplace("Hello");

        auto list = (TSharedPtr<TList<String>>) array;

        list->emplace("World");

        TWeakPtr<TArray<String>> weakArray = array;
        auto weakList = (TWeakPtr<TList<String>>) weakArray;

        auto link = weakList.lock();
        if (!link.isNull())
        {
            for (auto string = list->begin(); string != nullptr; string = list->next())
            {
                OutputDevice::printf("%s \n", string->get());
            }
        }
    }

    static void ObjectBaseTest3()
    {
        ResourceHandleData _data;
        TSharedPtr<ResourceHandleData> data(&_data, nullptr);
    }

    static void ObjectBaseTest4()
    {
        Object object;
        Component component;
        CameraComponent camera;

        OutputDevice::printf("%s %u \n", object.getType().get(), object.getTypeId());
        OutputDevice::printf("%s %u \n", component.getType().get(), component.getTypeId());
        OutputDevice::printf("%s %u \n", camera.getType().get(), camera.getTypeId());
    }

    static void run()
    {
        //ObjectBaseTest1();
        //ObjectBaseTest2();
        ObjectBaseTest3();
        ObjectBaseTest4();
    }

};


#endif //BERSERK_OBJECTSTEST_H
