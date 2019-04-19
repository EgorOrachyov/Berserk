//
// Created by Egor Orachyov on 13.04.2019.
//

#include "Managers/DebugDrawManager.h"

namespace Berserk::Render
{

    DebugDrawManager::DebugDrawManager(IAllocator *allocator)
            : mQueue1(INITIAL_PRIMITIVES_MAX_COUNT, allocator),
              mQueue2(INITIAL_PRIMITIVES_MAX_COUNT, allocator)
    {
        mCurrentRender = &mQueue1;
        mCurrentSubmit = &mQueue2;

        PUSH("DebugDrawManager: initialize");
    }

    DebugDrawManager::~DebugDrawManager()
    {
        PUSH("DebugDrawManager: de-initialize");
    }

    void DebugDrawManager::update()
    {
        std::lock_guard<std::mutex> lock(mMutex);

        // Swap submit and render queues
        auto tmp = mCurrentSubmit;
        mCurrentSubmit = mCurrentRender;
        mCurrentRender = tmp;

        // Mark as empty
        mCurrentSubmit->reset();
    }

    void DebugDrawManager::submit(const AABB &box, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Sphere &sphere, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Point &position, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Mat4x4f &transformation, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Point &start, const Point &end, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Point &A, const Point &B, const Point &C, const Color &color, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

    }

    void DebugDrawManager::submit(const Point &position, const char *text, const Color &color, float32 scale, bool depthTest)
    {
        std::lock_guard<std::mutex> lock(mMutex);

        DrawRequest request;

        request.mType = eDRT_TEXT;
        request.mTextPosition = position;
        request.mDepthTest = depthTest;
        request.mTextScale = scale;
        request.mColor = color;
        request.mStringBuffer = text;

        mCurrentSubmit->add(request);
    }

    const DebugDrawManager::Color DebugDrawManager::WHITE  = Color(1.0f, 1.0f, 1.0f);
    const DebugDrawManager::Color DebugDrawManager::RED    = Color(1.0f, 0.0f, 1.0f);
    const DebugDrawManager::Color DebugDrawManager::GREEN  = Color(0.0f, 1.0f, 0.0f);
    const DebugDrawManager::Color DebugDrawManager::BLUE   = Color(0.0f, 0.0f, 1.0f);
    const DebugDrawManager::Color DebugDrawManager::YELLOW = Color(1.0f, 1.0f, 0.0f);

} // namespace Berserk::Render