//
// Created by Egor Orachyov on 28.06.2019.
//

#ifndef BERSERK_CLOCK_H
#define BERSERK_CLOCK_H

#include <ctime>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>

namespace Berserk
{

    /**
     * Clock class to get calendar time base on std time
     * to get current info about data, time and year.
     */
    class CORE_EXPORT Clock
    {
    public:

        /** To measure elapsed time */
        typedef std::chrono::duration<float64> Duration;

        /** System clock to measure time */
        typedef std::chrono::system_clock SystemClock;

        /** Type to store time (calendar) */
        typedef std::chrono::system_clock::time_point TimePoint;

    public:

        Clock() : mCurrentPoint(SystemClock::now())
        { }

        ~Clock() = default;

        /** Update current time stamp */
        void update()
        {
            TimePoint now = SystemClock::now();
            mElapsedTime = std::chrono::duration_cast<Duration>(now - mCurrentPoint).count();
            mCurrentPoint = now;
        }

        /** @return Elapsed time as double value */
        float64 getElapsed() const { return mElapsedTime; }

        /** @return Elapsed time as float value */
        float32 getElapsedf() const { return (float32) mElapsedTime; }

        /** @return Time point since last update */
        TimePoint getTimePoint() const { return mCurrentPoint; }

        /** @return Current date-time string representation in format YYYY_MM_DD__HH_MM_SS */
        Name toString() const
        {
            std::time_t time = SystemClock::to_time_t(mCurrentPoint);
            std::tm tstruct = *std::localtime(&time);
            char buffer[SIZE_128];
            Printer::print(buffer, SIZE_128, "%i_%i_%i__%i_%i_%i",
                           tstruct.tm_year + 1900, tstruct.tm_mon + 1,
                           tstruct.tm_mday, tstruct.tm_hour, tstruct.tm_min, tstruct.tm_sec);

            return Name(buffer);
        }

    private:

        /** Elapsed time since last update */
        float64 mElapsedTime = 0;

        /** Current time */
        TimePoint mCurrentPoint;

    };

} // namespace Berserk

#endif //BERSERK_CLOCK_H