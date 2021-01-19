/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
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
    LinearAllocator<> allocator(Platform::Memory::MiB);

    size_t N = 20000;
    Array<String*> objects;

    for (auto i = 0; i < N; i++) {
        auto string = new (allocator.Allocate(sizeof(String))) String(String::From(i * 3 + 124));
        objects.Add(string);
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, allocator.GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(*objects[i], String::From(i * 3 + 124));
    }

    for (auto i = 0; i < N; i++) {
        objects[i]->~String();
        allocator.Deallocate(objects[i]);
    }

    allocator.Reset();
}

TEST_F(MemoryFixture, LinearAllocEmbedded) {
    LinearAllocator<> allocator(Platform::Memory::MiB);
    Array<String, LinearAllocator<>> array(std::move(allocator));

    size_t N = 5000;

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::From(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::From(i * i + i * 153 - 24));
    }

    array.Clear();
}

TEST_F(MemoryFixture, LinearAllocEmbeddedReused) {
    LinearAllocator<> allocator(Platform::Memory::MiB);
    Array<String, LinearAllocator<>> array(std::move(allocator));

    size_t N = 5000;

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::From(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::From(i * i + i * 153 - 24));
    }

    array.ClearAndReleaseMemory();
    array.GetAlloc().Reset();

    array.EnsureToAdd(N);

    for (auto i = 0; i < N; i++) {
        array.Emplace(String::From(i * i + i * 153 - 24));
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Allocated: {0} objects {1} bytes"), N, array.GetAlloc().GetAllocatedSize());

    for (auto i = 0; i < N; i++) {
        EXPECT_EQ(array[i], String::From(i * i + i * 153 - 24));
    }
}

BERSERK_GTEST_MAIN