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

BERSERK_GTEST_MAIN