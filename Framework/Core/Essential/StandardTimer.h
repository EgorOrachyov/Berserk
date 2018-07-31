//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_STANDARDTIMER_H
#define BERSERKENGINE_STANDARDTIMER_H

#include "Timer.h"

// todo: should be rewritten

namespace Berserk
{
    /**
     *  @brief Standard C timer based on system clock()
     */
    class StandardTimer : public Timer
    {
    public:

        StandardTimer() : Timer()
        {
            mStart = 0;
            mEnd = 0;
        }

        /**
         *  @brief Should be called before usage
         */
        void init() override
        {
            mStart = (UINT64)clock();
            mEnd = mStart;
        }

        /**
         *  @brief Start counting time
         */
        void start() override
        {
            mStart = (UINT64)clock();
        }

        /**
         *  @brief Stop counting time
         */
        void stop() override
        {
            mEnd = (UINT64)clock();
        }

        /**
         *  @brief Set internal value to zero
         */
        void reset() override
        {
            mStart = 0;
            mEnd = 0;
        }

        /**
         * @brief Get last milliseconds (from start);
         *
         * @return int64 value of milliseconds
         */
        UINT64 getMilliseconds() override
        {
            return (UINT64)clock() - mStart;
        }

        /**
         * @brief Get last seconds (from start);
         *
         * @return float64 value of milliseconds
         */
        FLOAT64 getSeconds() override
        {
            return ((FLOAT64)((UINT64)clock() - mStart)) / CLOCKS_PER_SEC;
        }

        /**
         * @brief Get last milliseconds (from start to end);
         *
         * @return int64 value of milliseconds
         */
        UINT64 getMillisecondsInInterval() override
        {
            return mEnd - mStart;
        }

        /**
         * @brief Get last seconds (from start to end);
         *
         * @return float64 value of milliseconds
         */
        FLOAT64 getSecondsInInterval() override
        {
            return (FLOAT64)(mEnd - mStart) / CLOCKS_PER_SEC;
        }

    private:

        UINT64 mStart;
        UINT64 mEnd;
    };

} // namespace Berserk

#endif //BERSERKENGINE_STANDARDTIMER_H
