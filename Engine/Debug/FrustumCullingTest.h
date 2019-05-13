//
// Created by Egor Orachyov on 09.02.2019.
//

#ifndef BERSERK_FRUSTUMCULLINGTEST_H
#define BERSERK_FRUSTUMCULLINGTEST_H

#include <chrono>
#include "Math/MathInclude.h"

#define UNBLOCK
#ifdef UNBLOCK

using namespace Berserk;

const uint32 OBJECTS_COUNT = 10000;

void SingleThread()
{
    AABB box(Vec3f(0,0,-5), 1.0f);
    Frustum frustum(Degrees(90.0f).radians().get(), 1.0f, 0.1f, 10.f, Vec3f(0,0,0), Vec3f(0,0,-1), Vec3f(0,1,0));

    auto start = std::chrono::high_resolution_clock::now();

    for(uint32 i = 0; i < OBJECTS_COUNT; i++)
    {
        frustum.inside(box);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elp = end - start;

    printf("Single Thread: %lfms \n", elp.count() * 1000.0);
}

void SingleThreadSIMD()
{
    AABB box(Vec3f(0,0,-5), 1.0f);
    Frustum frustum(Degrees(90.0f).radians().get(), 1.0f, 0.1f, 10.f, Vec3f(0, 0, 0), Vec3f(0, 0, -1), Vec3f(0, 1, 0));

    float32 result[OBJECTS_COUNT];
    ArrayList<AABB> source(OBJECTS_COUNT);

    for (uint32 i = 0; i < OBJECTS_COUNT; i++)
    {
        source += box;
    }

    auto start = std::chrono::high_resolution_clock::now();
    frustum.inside_SIMD(source.get(), result, OBJECTS_COUNT);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elp = end - start;

    printf("Single Thread SSE: %lfms \n", elp.count() * 1000.0);
}

void MultiThread()
{
    class Job : public IRunnable
    {
    public:

        Job(uint32 c, AABB b, Frustum f)
                : count(c), box(b), frustum(f) {}

        int32 run() override
        {
            for (uint32 i = 0; i < count; i++)
            {
                auto result = frustum.inside(box);
            }
        }

        uint32 count;
        AABB box;
        Frustum frustum;

    };

    ThreadPool executor;
    AABB box(Vec3f(0,0,-5), 1.0f);
    Frustum frustum(Degrees(90.0f).radians().get(), 1.0f, 0.1f, 10.f, Vec3f(0,0,0), Vec3f(0,0,-1), Vec3f(0,1,0));

    for(uint32 i = 0; i < 10000; i++) Thread::yield();

    uint32 count = OBJECTS_COUNT / 4;

    Job list[] =
            {
                    Job(count, box, frustum),
                    Job(count, box, frustum),
                    Job(count, box, frustum),
                    Job(count, box, frustum)
            };


    auto start = std::chrono::high_resolution_clock::now();

    for(uint32 i = 0; i < 3; i++)
    {
        executor.submit(&list[i], nullptr);
    }
    list[3].run();
    executor.join();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elp = end - start;

    printf("Multi-Thread: %lfms [Threads count: %u]\n", elp.count() * 1000.0, executor.THREADS_COUNT + 1);

    executor.shutdown();
}

void MultiThreadSIMD()
{
    class Job : public IRunnable
    {
    public:

        Job(uint32 c, uint32 s, AABB* b, float32* r, Frustum f)
                : count(c), start(s), box(b), result(r), frustum(f) {}

        int32 run() override
        {
            frustum.inside_SIMD(&box[start], &result[start], count);
        }

        uint32 count;
        uint32 start;
        AABB* box;
        float32* result;
        Frustum frustum;

    };

    ThreadPool executor;
    AABB box(Vec3f(0,0,-5), 1.0f);
    Frustum frustum(Degrees(90.0f).radians().get(), 1.0f, 0.1f, 10.f, Vec3f(0,0,0), Vec3f(0,0,-1), Vec3f(0,1,0));

    float32 result[OBJECTS_COUNT];
    ArrayList<AABB> source(OBJECTS_COUNT);

    for (uint32 i = 0; i < OBJECTS_COUNT; i++)
    {
        source += box;
    }

    for(uint32 i = 0; i < 10000; i++) Thread::yield();

    uint32 count = OBJECTS_COUNT / 4;

    Job list[] =
    {
            Job(count, 0,         source.get(), result, frustum),
            Job(count, count,     source.get(), result, frustum),
            Job(count, count * 2, source.get(), result, frustum),
            Job(count, count * 3, source.get(), result, frustum)
    };


    auto start = std::chrono::high_resolution_clock::now();

    for(uint32 i = 0; i < 3; i++)
    {
        executor.submit(&list[i], nullptr);
    }
    list[3].run();
    executor.join();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elp = end - start;

    printf("Multi-Thread SSE: %lfms [Threads count: %u]\n", elp.count() * 1000.0, executor.THREADS_COUNT + 1);

    executor.shutdown();
}

void FrustumCullingPerformance()
{
    printf("\n");

    LogManager::getSingleton().setVerbosity(NoLogging);

    printf("Frustum Culling Performance test\n");
    printf("Object: AABB Count: %u\n", OBJECTS_COUNT);

    SingleThread();
    SingleThreadSIMD();
    MultiThread();
    MultiThreadSIMD();

    LogManager::getSingleton().setVerbosity(Display);

    printf("\n");
}

#endif

#endif //BERSERK_FRUSTUMCULLINGTEST_H
