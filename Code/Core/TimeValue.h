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

namespace Berserk {
    class TimeValue {
    public:
        TimeValue() = default;
        ~TimeValue() = default;
        TimeValue(const TimeValue& other) = default;
        TimeValue(TimeValue&& other) noexcept = default;

        void setSeconds(float64 value) {
            using namespace std::chrono;
            auto dur = (int64)(value * 1000.f * 1000.f);
            high_resolution_clock::time_point tp{microseconds{dur}};
            mTimePoint = tp;
        }

        void setMilliseconds(float64 value) {
            using namespace std::chrono;
            auto dur = (int64)(value * 1000.f);
            high_resolution_clock::time_point tp{microseconds{dur}};
            mTimePoint = tp;
        }

        void setMicroseconds(float64 value) {
            using namespace std::chrono;
            auto dur = (int64)(value);
            high_resolution_clock::time_point tp{microseconds{dur}};
            mTimePoint = tp;
        }

        float64 getSeconds() const {
            using namespace std::chrono;
            auto sec = time_point_cast<microseconds>(mTimePoint);
            auto dur = sec.time_since_epoch();
            return (float32)((float64)dur.count() / 1000.0f / 1000.0f);
        }

        float64 getMilliseconds() const {
            using namespace std::chrono;
            auto sec = time_point_cast<microseconds>(mTimePoint);
            auto dur = sec.time_since_epoch();
            return (float32)((float64)dur.count() / 1000.0f);
        }

        float64 getMicroseconds() const {
            using namespace std::chrono;
            auto sec = time_point_cast<microseconds>(mTimePoint);
            auto dur = sec.time_since_epoch();
            return (float32)dur.count();
        }

        TimeValue operator-(const TimeValue& other) const {
            using namespace std::chrono;
            high_resolution_clock::time_point tp(mTimePoint - other.mTimePoint);
            TimeValue result;
            result.mTimePoint = tp;
            return result;
        }

        TimeValue operator+(const TimeValue& other) const {
            using namespace std::chrono;
            auto a = mTimePoint.time_since_epoch().count();
            auto b = other.mTimePoint.time_since_epoch().count();
            high_resolution_clock::time_point tp(microseconds(a + b));

            TimeValue result;
            result.mTimePoint = tp;
            return result;
        }

        static TimeValue now() {
            using namespace std::chrono;
            auto tp = high_resolution_clock::now();
            TimeValue result;
            result.mTimePoint = tp;
            return result;
        }

    private:
        using Clock = std::chrono::high_resolution_clock;
        using Duration = std::chrono::microseconds;
        using TimePoint = std::chrono::high_resolution_clock::time_point;
        TimePoint mTimePoint;
    };
}

#endif //BERSERK_TIMEVALUE_H