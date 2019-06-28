//
// Created by Egor Orachyov on 28.06.2019.
//

#ifndef BERSERK_TIMER_H
#define BERSERK_TIMER_H

#include <chrono>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * High resolution std based timer to measure elapsed time
     * between frames or events. Use update function to update timer.
     */
    class CORE_EXPORT Timer
    {
    public:

        /** To measure elapsed time */
        typedef std::chrono::duration<float64> Duration;

        /** High resolution clock */
        typedef std::chrono::high_resolution_clock HighResClock;

        /** System clock to measure time */
        typedef std::chrono::system_clock SystemClock;

        /** High resolution timer time point */
        typedef std::chrono::high_resolution_clock::time_point TimePoint;

    public:

        /** Initialize with current time point */
        Timer() : mCurrentPoint(HighResClock::now())
        { }

        ~Timer() = default;

        /** Updates elapsed time and current point. Should be called each frame */
        void update()
        {
            TimePoint now = HighResClock::now();
            mElapsedTime = std::chrono::duration_cast<Duration>(now - mCurrentPoint).count();
            mCurrentPoint = now;
        }

        /** @return Elapsed time as double value */
        float64 getElapsed() const { return mElapsedTime; }

        /** @return Elapsed time as float value */
        float32 getElapsedf() const { return (float32) mElapsedTime; }

        /** @return Time point since last update */
        TimePoint getTimePoint() const { return mCurrentPoint; }

    protected:

        /** Elapsed time since last update */
        float64 mElapsedTime = 0;

        /** Now */
        TimePoint mCurrentPoint;

    };

} // namespace Berserk

#endif //BERSERK_TIMER_H