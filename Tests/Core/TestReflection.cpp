/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <Reflection/Variant.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Reflection)
{
    BERSERK_TEST_COND(Variant, true)
    {
        TArray<Variant> items = { "apple", "banana", "orange" };
        Variant container(items);

        for (auto& item: container.getArray()) {
            printf("Item: %s\n", item.getString().data());
        }

        container.asMap();
        container.getMap().emplace("apple", 1);
        container.getMap().emplace("banana", "yellow");
        container.getMap().emplace("orange", 11.01f);

        printf("%s\n", container.toString().data());

        printf("String size: %u\n",  (uint32) sizeof(Variant::String));
        printf("Array size: %u\n",   (uint32) sizeof(Variant::Array));
        printf("Map size: %u\n",     (uint32) sizeof(Variant::Map));
        printf("Variant size: %u\n", (uint32) sizeof(Variant));

    };
}