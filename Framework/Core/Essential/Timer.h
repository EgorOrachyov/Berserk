//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_TIMER_H
#define BERSERKENGINE_TIMER_H

#include "Common.h"
#include "Types.h"

namespace Berserk
{

    /**
     *  @brief Virtual timer
     */
    class Timer
    {
    public:

        /**
         *  @brief Should be called before usage
         */
        virtual void init() = 0;

        /**
         *  @brief Start counting time
         */
        virtual void start() = 0;

        /**
         *  @brief Stop counting time
         */
        virtual void stop() = 0;

        /**
         *  @brief Set internal value to zero
         */
        virtual void reset() = 0;

        /**
         * @brief Get last milliseconds (from start);
         *
         * @return int64 value of milliseconds
         */
        virtual UINT64 getMilliseconds() = 0;

        /**
         * @brief Get last seconds (from start);
         *
         * @return float64 value of milliseconds
         */
        virtual FLOAT64 getSeconds() = 0;

        /**
         * @brief Get last milliseconds (from start to end);
         *
         * @return int64 value of milliseconds
         */
        virtual UINT64 getMillisecondsInInterval() = 0;

        /**
         * @brief Get last seconds (from start to end);
         *
         * @return float64 value of milliseconds
         */
        virtual FLOAT64 getSecondsInInterval() = 0;

    };

} // namespace Berserk

#endif //BERSERKENGINE_TIMER_H
