/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_DATETIME_HPP
#define BERSERK_DATETIME_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Strings/StringBuilder.hpp>

namespace Berserk {

    /** Defines how date or time must be interpreted */
    enum class TimeType {
        Local,
        Global
    };

    class Date {
    public:

        enum class Weekday {
            Sunday = 0,
            Monday,
            Tuesday,
            Wednesday,
            Thursday,
            Friday,
            Saturday
        };

        static const uint32 TotalWeekdays = 7;

        static Weekday GetWeekday(uint32 id) {
            static const Weekday Weekdays[] = {
                Weekday::Sunday,
                Weekday::Monday,
                Weekday::Tuesday,
                Weekday::Wednesday,
                Weekday::Thursday,
                Weekday::Friday,
                Weekday::Saturday
            };

            return id < TotalWeekdays? Weekdays[id]: Weekday::Sunday;
        }

        enum class Month {
            January = 0,
            February,
            March,
            April,
            May,
            June,
            July,
            August,
            September,
            October,
            November,
            December
        };

        static const uint32 TotalMonths = 12;

        static Month GetMonth(uint32 id) {
            static const Month Months[] = {
                Month::January,
                Month::February,
                Month::March,
                Month::April,
                Month::May,
                Month::June,
                Month::July,
                Month::August,
                Month::September,
                Month::October,
                Month::November,
                Month::December
            };

            return id < TotalMonths? Months[id]: Month::January;
        }

        static uint32 GetMonthNumber(Month month) {
            uint32 num = (uint32) month + 1;
            return num <= TotalMonths? num: TotalMonths;
        }

        Date() = default;

        explicit Date(Weekday weekday, Month month, uint32 dayYear, uint32 dayMonth, uint32 year)
            : weekday(weekday), month(month), dayYear(dayYear), dayMonth(dayMonth), year(year) {

        }

        uint32 GetMonthNumber() const {
            return Date::GetMonthNumber(month);
        }

        String ToString() const {
            // todo: switch to fixed string builder
            String string;
            string += String::From(year);
            string += BERSERK_TEXT(".");
            string += String::From(GetMonthNumber());
            string += BERSERK_TEXT(".");
            string += String::From(dayMonth);
            return string;
        }

        Weekday weekday = Weekday::Sunday;
        Month month = Month::January;
        uint32 dayYear = 0;
        uint32 dayMonth = 0;
        uint32 year = 0;
    };

    class Time {
    public:

        Time() = default;

        explicit Time(uint32 hour, uint32 min, uint32 sec)
            : hour(hour), min(min), sec(sec) {

        }

        String ToString() const {
            // todo: switch to fixed string builder
            String string;
            string += String::From(hour);
            string += BERSERK_TEXT(":");
            string += String::From(min);
            string += BERSERK_TEXT(":");
            string += String::From(sec);
            return string;
        }

        uint32 hour = 0;
        uint32 min = 0;
        uint32 sec = 0;
    };

    class TimeStamp {
    public:
        explicit TimeStamp(uint64 native)
            : native(native) {

        }

        uint64 native = 0;
    };

    template<>
    class TextPrint<Date::Weekday> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Date::Weekday& weekday) const {
            static const char* Weekdays[] = {
                BERSERK_TEXT("Sunday"),
                BERSERK_TEXT("Monday"),
                BERSERK_TEXT("Tuesday"),
                BERSERK_TEXT("Wednesday"),
                BERSERK_TEXT("Thursday"),
                BERSERK_TEXT("Friday"),
                BERSERK_TEXT("Saturday")
            };

            auto id = (uint32) weekday;

            if (id < Date::TotalWeekdays) {
                stream.Add(Weekdays[id]);
            }
            else {
                stream.Add(BERSERK_TEXT("Unknown"));
            }
        }
    };

    template<>
    class TextPrint<Date::Month> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Date::Month& month) const {
            static const char* Months[] = {
                BERSERK_TEXT("January"),
                BERSERK_TEXT("February"),
                BERSERK_TEXT("March"),
                BERSERK_TEXT("April"),
                BERSERK_TEXT("May"),
                BERSERK_TEXT("June"),
                BERSERK_TEXT("July"),
                BERSERK_TEXT("August"),
                BERSERK_TEXT("September"),
                BERSERK_TEXT("October"),
                BERSERK_TEXT("November"),
                BERSERK_TEXT("December")
            };

            auto id = (uint32) month;

            if (id < Date::TotalMonths) {
                stream.Add(Months[id]);
            }
            else {
                stream.Add(BERSERK_TEXT("Unknown"));
            }
        }
    };

}

#endif //BERSERK_DATETIME_HPP