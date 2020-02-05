/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TIMEVALUE_H
#define BERSERK_TIMEVALUE_H

#include <Typedefs.h>
#include <chrono>
#include <thread>

namespace Berserk {
    class TimeValue {
    public:
        TimeValue() = default;
        ~TimeValue() = default;
        TimeValue(const TimeValue& other) = default;
        TimeValue(TimeValue&& other) noexcept = default;

        TimeValue& setSeconds(float64 value) {
            mTimePoint = (int64)(value * 1000.0f * 1000.0f);
            return *this;
        }

        TimeValue& setMilliseconds(float64 value) {
            mTimePoint = (int64)(value * 1000.0f);
            return *this;
        }

        TimeValue& setMicroseconds(float64 value) {
            mTimePoint = (int64)value;
            return *this;
        }

        float64 getSeconds() const {
            return (float32)((float64)mTimePoint / 1000.0f / 1000.0f);
        }

        float64 getMilliseconds() const {
            return (float32)((float64)mTimePoint / 1000.0f);
        }

        float64 getMicroseconds() const {
            return (float32)mTimePoint;
        }

        TimeValue operator-(const TimeValue& other) const {
            TimeValue result{};
            result.mTimePoint = mTimePoint - other.mTimePoint;
            return result;
        }

        TimeValue operator+(const TimeValue& other) const {
            TimeValue result{};
            result.mTimePoint = mTimePoint + other.mTimePoint;
            return result;
        }

        bool operator>(const TimeValue& other) const  { return mTimePoint > other.mTimePoint; }
        bool operator<(const TimeValue& other) const  { return mTimePoint < other.mTimePoint; }
        bool operator>=(const TimeValue& other) const { return mTimePoint >= other.mTimePoint; }
        bool operator<=(const TimeValue& other) const { return mTimePoint <= other.mTimePoint; }
        bool operator==(const TimeValue& other) const { return mTimePoint == other.mTimePoint; }
        bool operator!=(const TimeValue& other) const { return mTimePoint != other.mTimePoint; }

        TimeValue& operator=(const TimeValue& other) = default;
        TimeValue& operator=(TimeValue&& other) = default;

        static TimeValue asSeconds(float64 s)       { return TimeValue().setSeconds(s); }
        static TimeValue asMilliseconds(float64 ms) { return TimeValue().setMilliseconds(ms); }
        static TimeValue asMicroseconds(float64 us) { return TimeValue().setMicroseconds(us); }

        /** @return Current time */
        static TimeValue now() {
            using namespace std::chrono;
            auto tp = high_resolution_clock::now();
            auto mc = time_point_cast<microseconds>(tp);
            auto dur = mc.time_since_epoch();

            TimeValue result{};
            result.mTimePoint = dur.count();
            return result;
        }

        /**
         * @warning For debugging and testing only
         * @return Waits specified duration and returns current time
         */
        static TimeValue wait(const TimeValue& until) {
            using namespace std::chrono;
            auto v = TimeValue::now();

            while (v < until) {
                v = TimeValue::now();
            }

            return v;
        }

    private:
        // Time point in microseconds
        int64 mTimePoint = 0;
    };
}

#endif //BERSERK_TIMEVALUE_H