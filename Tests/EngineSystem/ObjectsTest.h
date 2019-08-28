//
// Created by Egor Orachyov on 2019-08-02.
//

#ifndef BERSERK_OBJECTSTEST_H
#define BERSERK_OBJECTSTEST_H

#include <Object/Object.h>
#include <Resource/ResourceHandle.h>

#include <Components/CameraComponent.h>
#include <Components/DirectionalLightComponent.h>
#include <Components/StaticMeshComponent.h>

using namespace Berserk;

class ObjectsTest
{
public:

    static void ObjectBaseTest1()
    {
        IAllocator& allocator = Allocator::get();
        auto object = allocator.mem_new<Object>();

        OutputDevice::printf("Run-time type name: %s, type id: %u \n",
                object->getType().get(), object->getTypeID());

        allocator.mem_destroy(object);

    }

    static void ObjectBaseTest2()
    {
        IAllocator& allocator = Allocator::get();

        auto _array = allocator.mem_new<TArray<String>>(allocator);
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

        OutputDevice::printf("%s %u \n", object.getType().get(), object.getTypeID());
        OutputDevice::printf("%s %u \n", component.getType().get(), component.getTypeID());
        OutputDevice::printf("%s %u \n", camera.getType().get(), camera.getTypeID());
    }

    static void ComponentsTest1()
    {
        StaticMeshComponent mesh;
        DirectionalLightComponent light;
        CameraComponent camera;

        OutputDevice::printf("Class: %s id: %u \n", mesh.getType_str(), mesh.getTypeID());
        OutputDevice::printf("Class: %s id: %u \n", light.getType_str(), light.getTypeID());
        OutputDevice::printf("Class: %s id: %u \n", camera.getType_str(), camera.getTypeID());

        OutputDevice::printf("Class: %s flags: %u \n", mesh.getType_str(), mesh.getDirtyFlags());
        OutputDevice::printf("Class: %s flags: %u \n", light.getType_str(), light.getDirtyFlags());
        OutputDevice::printf("Class: %s flags: %u \n", camera.getType_str(), camera.getDirtyFlags());
    }

    static void run()
    {
        //ObjectBaseTest1();
        //ObjectBaseTest2();
        //ObjectBaseTest3();
        //ObjectBaseTest4();
        ComponentsTest1();
    }

};


#endif //BERSERK_OBJECTSTEST_H
