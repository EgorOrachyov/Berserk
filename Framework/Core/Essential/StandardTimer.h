//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_STANDARDTIMER_H
#define BERSERKENGINE_STANDARDTIMER_H

#include "Timer.h"

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
        virtual void init() override
        {
            mStart = (int64)clock();
            mEnd = mStart;
        }

        /**
         *  @brief Start counting time
         */
        virtual void start() override
        {
            mStart = (int64)clock();
        }

        /**
         *  @brief Stop counting time
         */
        virtual void stop() override
        {
            mEnd = (int64)clock();
        }

        /**
         *  @brief Set internal value to zero
         */
        virtual void reset() override
        {
            mStart = 0;
            mEnd = 0;
        }

        /**
         * @brief Get last milliseconds (from start);
         *
         * @return int64 value of milliseconds
         */
        virtual int64 getMilliseconds() override
        {
            return (int64)clock() - mStart;
        }

        /**
         * @brief Get last seconds (from start);
         *
         * @return float64 value of milliseconds
         */
        virtual float64 getSeconds() override
        {
            return (float64)((int64)clock() - mStart) / CLOCKS_PER_SEC;
        }

        /**
         * @brief Get last milliseconds (from start to end);
         *
         * @return int64 value of milliseconds
         */
        virtual int64 getMillisecondsInInterval() override
        {
            return mEnd - mStart;
        }

        /**
         * @brief Get last seconds (from start to end);
         *
         * @return float64 value of milliseconds
         */
        virtual float64 getSecondsInInterval() override
        {
            return (float64)(mEnd - mStart) / CLOCKS_PER_SEC;
        }

    private:

        int64 mStart;
        int64 mEnd;
    };

} // namespace Berserk

#endif //BERSERKENGINE_STANDARDTIMER_H
