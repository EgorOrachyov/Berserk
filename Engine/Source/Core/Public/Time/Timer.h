//
// Created by Egor Orachyov on 10.02.2019.
//

#ifndef BERSERK_TIMER_H
#define BERSERK_TIMER_H

#include <chrono>
#include "Public/Misc/Types.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * High resolution std chrono based timer.
     * Allows to calculate elapsed time between frames. Also provides
     * limitation for elapsed time to avoid huge bugs in time stamps
     */
    class CORE_API Timer
    {
    public:

        /**
         * Consider 1/30 of second to be the top border for calculation elapsed
         * time. It means that if elapsed time more than Limitation, then its value will
         * be replaced by that limitation
         */
        const static float64 DEFAULT_LIMITATION;

        /** High resolution timer time point */
        typedef std::chrono::high_resolution_clock::time_point TimePoint;

    public:

        /** Initializes timer and starts it */
        Timer(float64 limitation = DEFAULT_LIMITATION)
        {
            mElapsed = 0;
            mLimitation = DEFAULT_LIMITATION;
            mTimePoint = std::chrono::high_resolution_clock::now();
        }

        Timer(const Timer& source) = default;

        ~Timer() = default;

        /** Explicit timer start */
        void start()
        {
            mTimePoint = std::chrono::high_resolution_clock::now();
        }

        /** Calculates elapsed and updates time point */
        void update()
        {
            TimePoint end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float64> elp = end - mTimePoint;

            if (elp.count() > mLimitation) mElapsed = mLimitation;
            else mElapsed = elp.count();

            mTimePoint = end;
        }

        /** @return Current high resolution timer time point */
        TimePoint now() const { return mTimePoint; }

        /** @return Delta time between last update */
        float64 elapsed() const { return mElapsed; }

        /** @return Current try elapsed time from the start call [or initialization] */
        float64 current()
        {
            TimePoint end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float64> elp = end - mTimePoint;

            return elp.count();
        }

    public:

        /** Global static timer */
        static Timer& getSingleton()
        {
            static Timer timer;
            return timer;
        }

    private:

        float64 mElapsed;
        float64 mLimitation;
        TimePoint mTimePoint;

    };

} // namespace Berserk

#endif //BERSERK_TIMER_H
