/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <IO/ArchiveFile.h>
#include <Platform/System.h>
#include <Reflection/Class.h>
#include <Reflection/Object.h>
#include <Reflection/ClassBuilder.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Reflection)
{

    BERSERK_TEST_COND(Variant, false)
    {
        TArray<Variant> items = { "apple", "banana", "orange" };
        Variant container(items);

        printf("%s\n", container.toString().data());

        container.asMap();
        container.getMap().emplace("apple", 1);
        container.getMap().emplace("banana", "yellow");
        container.getMap().emplace("orange", 11.01f);

        printf("%s\n", container.toString().data());

        printf("String size: %u\n",  (uint32)sizeof(Variant::String));
        printf("Array size: %u\n",   (uint32)sizeof(Variant::Array));
        printf("Map size: %u\n",     (uint32)sizeof(Variant::Map));
        printf("Variant size: %u\n", (uint32)sizeof(Variant));
    };

    BERSERK_TEST_COND(VariantSerialization, false)
    {
        TArray<Variant> items = { "apple", "banana", "orange", false, 1.0101f, 1000 };
        Variant container(items);

        auto file = System::getSingleton().openFile("variant_array.bin", EFileMode::Write);
        ArchiveFile archive(file);

        archive << container;
    };

    BERSERK_TEST_COND(VariantDeserialization, false)
    {
        Variant container;

        auto file = System::getSingleton().openFile("variant_array.bin", EFileMode::Read);
        ArchiveFile archive(file);

        archive >> container;

        printf("%s\n", container.toString().data());
    };

    BERSERK_TEST_COND(VariantMap, false)
    {
        Variant object;
        auto &map = object.getMap();

        map["apple"] = 132.02f;
        map["banana"] = 1111;
        map["orange"] = false;
        map["potato"] = {"one","two", {"three","four","five"}, {"list", 732, false, false, -0.032f}};
        map[543.0f] = "float";

        printf("%s\n", object.toString().data());

        printf("(key: %s, value: %s) \n", "apple", map["apple"].toString().data());
        printf("(key: %s, value: %s) \n", "543.0f", map[543.0f].toString().data());
        printf("Hash: %u \n", object.hash());
        printf("Hash: %u \n", map["potato"].hash());
    };

    BERSERK_TEST_COND(VariantMapPair, false)
    {
        Variant object;
        auto &map = object.getMap();

        map[{"0","0"}] = 1;
        map[{"0","1"}] = 1;
        map[{"1","1"}] = 1;
        map[{"1","1"}].getInt() += 1;

        printf("%s\n", object.toString().data());
    };

    BERSERK_TEST_COND(VariantObject, false)
    {
        Variant object;
        auto& self = object.getMap();

        self["name"] = "Some object";
        self["type"] = "Object";
        self["script"] = "Scripts/Object.lua";

        auto& state = self["state"].getMap();

        state["health"] = 100;
        state["position"] = { 0, 0};
        state["stamina"] = 100;
        state["xp"] = 0;
        state["created"] = false;

        printf("Object dump: %s\n", object.toString().data());
    };

    BERSERK_TEST_COND(ClassReflection, true)
    {
        class MyObject : public Object {
            BERSERK_CLASS(TestObject,Object)

        public:
            ~MyObject() override = default;

            int32 print(const CString& message) const {
                printf("value: %i message: %s\n", value, message.data());
                return value * 2;
            }

            static void registerInfo() {
                ClassBuilder<MyObject> builder;

                builder
                    .registerClass()
                    .addProperty("value", EVariantType::Int)
                    .addMethod(Property(EVariantType::Int), "print", Property("message", EVariantType::String), &MyObject::print);

            }

        private:
            int32 value = 222;
        };

        ClassManager manager;
        Object::registerInfo();
        MyObject::registerInfo();

        MyObject object;
        Class& objectClass = Object::getClassStatic();
        Class& myObjectClass = object.getClass();

        objectClass.showDebugInfo();
        myObjectClass.showDebugInfo();

        Variant ret;
        Variant message = "Hello reflection";
        auto& method = myObjectClass.getMethod("print");
        auto status = method.call(object,{ &message },ret);

        if (status != EError::OK) {
            printf("Failed to call method");
        }

        printf("Result: %s\n", ret.toString().data());
    };
}
