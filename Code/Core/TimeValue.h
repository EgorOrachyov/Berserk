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
#include <Platform/Thread.h>

#include <chrono>

namespace Berserk {
    class TimeValue {
    public:
        TimeValue() = default;
        ~TimeValue() = default;
        TimeValue(const TimeValue& other) = default;
        TimeValue(TimeValue&& other) noexcept = default;

        TimeValue& setSeconds(double value) {
            mMicroseconds = (int64)(value * 1000.0f * 1000.0f);
            return *this;
        }

        TimeValue& setMilliseconds(double value) {
            mMicroseconds = (int64)(value * 1000.0f);
            return *this;
        }

        TimeValue& setMicroseconds(double value) {
            mMicroseconds = (int64)value;
            return *this;
        }

        TimeValue& setNanoseconds(double value) {
            mMicroseconds = (int64)(value / 1000.0f);
            return *this;
        }

        double getSeconds() const {
            return (float)((double)mMicroseconds / 1000.0f / 1000.0f);
        }

        double getMilliseconds() const {
            return (float)((double)mMicroseconds / 1000.0f);
        }

        double getMicroseconds() const {
            return (float)mMicroseconds;
        }

        double getNanoseconds() const {
            return (float)((double)mMicroseconds * 1000.0f);
        }

        const int64& getRawValue() const {
            return mMicroseconds;
        }

        int64& getRawValue() {
            return mMicroseconds;
        }

        TimeValue operator-(const TimeValue& other) const {
            TimeValue result{};
            result.mMicroseconds = mMicroseconds - other.mMicroseconds;
            return result;
        }

        TimeValue operator+(const TimeValue& other) const {
            TimeValue result{};
            result.mMicroseconds = mMicroseconds + other.mMicroseconds;
            return result;
        }

        /** @return Time value as string YYYY:MM:DD HH:MM:SS */
        CString toFormatString() const;

        bool operator>(const TimeValue& other) const  { return mMicroseconds > other.mMicroseconds; }
        bool operator<(const TimeValue& other) const  { return mMicroseconds < other.mMicroseconds; }
        bool operator>=(const TimeValue& other) const { return mMicroseconds >= other.mMicroseconds; }
        bool operator<=(const TimeValue& other) const { return mMicroseconds <= other.mMicroseconds; }
        bool operator==(const TimeValue& other) const { return mMicroseconds == other.mMicroseconds; }
        bool operator!=(const TimeValue& other) const { return mMicroseconds != other.mMicroseconds; }

        TimeValue& operator=(const TimeValue& other) = default;
        TimeValue& operator=(TimeValue&& other) = default;

        static TimeValue asSeconds(double s)       { return TimeValue().setSeconds(s); }
        static TimeValue asMilliseconds(double ms) { return TimeValue().setMilliseconds(ms); }
        static TimeValue asMicroseconds(double us) { return TimeValue().setMicroseconds(us); }
        static TimeValue asNanoseconds(double ns) { return TimeValue().setNanoseconds(ns); }

        /** @return Current time point (not actual time) */
        static TimeValue now() {
            using namespace std::chrono;
            auto tp = high_resolution_clock::now();
            auto mc = time_point_cast<microseconds>(tp);
            auto dur = mc.time_since_epoch();

            TimeValue result{};
            result.mMicroseconds = dur.count();
            return result;
        }

        /** @return Current time point (as actual system time) */
        static TimeValue nowAsTime() {
            using namespace std::chrono;
            auto tp = system_clock::now();
            auto mc = time_point_cast<microseconds>(tp);
            auto dur = mc.time_since_epoch();

            TimeValue result{};
            result.mMicroseconds = dur.count();
            return result;
        }

        /**
         * @warning For debugging and testing only
         * @return Waits until specified time point and returns current time
         */
        static TimeValue wait(const TimeValue& until) {
            auto v = TimeValue::now();

            while (v < until) {
                Thread::yieldCurrent();
                v = TimeValue::now();
            }
            return v;
        }

    private:
        // Time point in microseconds
        int64 mMicroseconds = 0;
    };
}

#endif //BERSERK_TIMEVALUE_H