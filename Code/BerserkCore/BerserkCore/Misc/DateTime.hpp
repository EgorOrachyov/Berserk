/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_DATETIME_HPP
#define BERSERK_DATETIME_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Misc/Contracts.hpp>

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
    class FormatPrint<Date::Weekday> {
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
    class FormatPrint<Date::Month> {
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