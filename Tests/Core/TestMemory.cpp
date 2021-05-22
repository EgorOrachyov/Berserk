/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Memory/LinearAllocator.hpp>
#include <BerserkCore/Debug/Debug.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(MemoryFixture)

TEST_F(MemoryFixture, PoolSetup) {
    PoolAllocator<> pool(sizeof(uint64));
}

TEST_F(MemoryFixture, PoolAllocate) {
    PoolAllocator<> pool(sizeof(uint64));

    Array<void*> mem;
    auto count = 100;

    for (auto i = 0; i < count; i++) {
        mem.Add(pool.Allocate(sizeof(uint64)));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated={0} Total={1}"), pool.GetAllocatedChunks(), pool.GetTotalChunks());

    for (auto p: mem) {
        pool.Deallocate(p);
    }
}

TEST_F(MemoryFixture, LinearAllocBasic) {
    LinearAllocator<> allocator(Memory::MiB);

    uint64 N = 20000;
    Array<String*> objects;

    for (auto i = 0; i < N; i++) {
        auto string = new (allocator.Allocate(sizeof(String))) String(String::Fromi32(i * 3 + 124));
        objects.Add(string);
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, (uint64) allocator.GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(*objects[i], String::Fromi32(i * 3 + 124));
    }

    for (auto i = 0; i < N; i++) {
        objects[i]->~String();
        allocator.Deallocate(objects[i]);
    }

    allocator.Reset();
}

TEST_F(MemoryFixture, LinearAllocEmbedded) {
    LinearAllocator<> allocator(Memory::MiB);
    Array<String, LinearAllocator<>> array(std::move(allocator));

    uint64 N = 5000;

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::Fromi32(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::Fromi32(i * i + i * 153 - 24));
    }

    array.Clear();
}

TEST_F(MemoryFixture, LinearAllocEmbeddedReused) {
    LinearAllocator<> allocator(Memory::MiB);
    Array<String, LinearAllocator<>> array(std::move(allocator));

    uint64 N = 5000;

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::Fromi32(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::Fromi32(i * i + i * 153 - 24));
    }

    array.ClearAndReleaseMemory();
    array.GetAlloc().Reset();

    array.EnsureToAdd(N);

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::Fromi32(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::Fromi32(i * i + i * 153 - 24));
    }
}

TEST_F(MemoryFixture, SharedPointerBasic) {
    SharedRef<String> r1 = SharedRef<String>::Make();
    *r1;

    *r1 = BERSERK_TEXT("Fancy message zß水🍌 :)");

    SharedPtr<String> p1{r1};
    SharedRef<String> r2 = p1.ToSharedRef();
    WeakPtr<String> w1{r1};

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), *r1);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), *r2);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), *p1);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("srfs={0} wrfs={1}"), r1.GetSharedRefs(), r1.GetWeakRefs());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("srfs={0} wrfs={1}"), p1.GetSharedRefs(), p1.GetWeakRefs());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("srfs={0} wrfs={1}"), w1.GetSharedRefs(), w1.GetWeakRefs());

    SharedPtr<String> p2 = w1.ToSharedPtr();

    ASSERT_TRUE(p2);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("srfs={0} wrfs={1}"), p2.GetSharedRefs(), p2.GetWeakRefs());

    ASSERT_EQ(p2.GetSharedRefs(), 2 + 2);
    ASSERT_EQ(p2.GetWeakRefs(), 1 + 1);

    w1 = nullptr;
    p1 = w1.ToSharedPtr();

    ASSERT_FALSE(w1);
    ASSERT_FALSE(p1);
}

TEST_F(MemoryFixture, SharedPointerContracts) {
    Map<WeakPtr<String>, int32> map;
    Array<SharedRef<String>> data;

    const char* array[] = {
        BERSERK_TEXT("Fancy short message zß水🍌 :)"),
        BERSERK_TEXT("Fancy long message zß水🍌 :)"),
        BERSERK_TEXT("Fancy the same message zß水🍌 :)"),
        BERSERK_TEXT("Fancy zß水🍌 :)"),
        BERSERK_TEXT("Fancy fancy zß水🍌 :)"),
        BERSERK_TEXT("Fancy привет zß水🍌 :)"),
    };

    for (Berserk::size_t i = 0; i < 6; i++) {
        int32 id = i;
        String name{array[i]};

        auto entry = SharedRef<String>::MakeMove(std::move(name));

        map.Add(WeakPtr<String>{entry}, id);
        data.Move(std::move(entry));
    }

    for (auto& e: map) {
        ASSERT_TRUE(e.GetFirst().ToSharedPtr());
    }

    for (int i = 5; i >= 0; i--) {
        if (i % 2 == 0) {
            data.Remove(i);
        }
    }

    int count = 0;

    for (auto& e: map) {
        auto p = e.GetFirst().ToSharedPtr();

        if (p) {
            count++;
            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), *p);
        }
    }

    ASSERT_TRUE(count == 3);
};

TEST_F(MemoryFixture, SharedPointerThreading) {
    auto total = 10000;
    auto message = SharedRef<String>::Make(BERSERK_TEXT("Fancy big lon привет message zß水🍌 :)"));

    auto t1 = ThreadManager::CreateThread(BERSERK_TEXT("T1"), [&](){
        ThreadManager::CurrentThreadSleep(100000);

        Array<SharedRef<String>> refs;
        Array<SharedPtr<String>> ptrs;
        Array<SharedPtr<String>> weaks;

        for (auto i = 0; i < total; i++) {
            refs.Emplace(message);
            ptrs.Emplace(message);
            weaks.Emplace(message);
        }

        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0} {1}"), message.GetSharedRefs(), message.GetWeakRefs());
    });

    auto t2 = ThreadManager::CreateThread(BERSERK_TEXT("T2"), [&](){
        Array<SharedRef<String>> refs;
        Array<SharedPtr<String>> ptrs;
        Array<SharedPtr<String>> weaks;

        for (auto i = 0; i < total; i++) {
            refs.Emplace(message);
            ptrs.Emplace(message);
            weaks.Emplace(message);
        }

        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0} {1}"), message.GetSharedRefs(), message.GetWeakRefs());
    });

    t1->Join();
    t2->Join();

    ASSERT_TRUE(message.IsUnique());
};

TEST_F(MemoryFixture, SharedFromThis) {
    class MyObject;

    class RegistryObject {
    public:
        void Register(const SharedRef<MyObject> &object) const {
            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), object);
        }
    };

    class MyObject: public SharedFromThis<MyObject> {
    public:
        Array<String> data1;
        Map<int, String> data2;

        void Register(RegistryObject& registryObject) {
            registryObject.Register(AsShared());
        }
    };

    auto registry = RegistryObject{};
    auto object = SharedRef<MyObject>::Make();
    object->AssignSelf(object);

    object->Register(registry);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), object);
}

BERSERK_GTEST_MAIN