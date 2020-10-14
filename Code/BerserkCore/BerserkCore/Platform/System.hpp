/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    /** System data and time info */
    struct DateTime {
        int32 year = 0;
        int32 month = 0;    /** Indexed from 0 */
        int32 dayWeek = 0;  /** Indexed from 0 */
        int32 dayMonth = 0; /** Indexed from 0 */
        int32 dayYear = 0;  /** Indexed from 0 */
        int32 hour = 0;     /** Indexed from 0 */
        int32 min = 0;      /** Indexed from 0 */
        int32 sec = 0;      /** Indexed from 0 */

        /** @return Time in format 'YYYY.MM.DD HH:MM:SS' */
        String ToString() const;
        /** @return Time in format 'YYYY-MM-DD_HH-MM-SS' */
        String ToStringConservative() const;
    };

    /** Abstracts underlying OS specifics */
    class System {
    public:
        virtual ~System() = default;
        virtual Allocator& GetAllocator() = 0;
        virtual DateTime GetDateTime() = 0;
        virtual TPtrShared<File> OpenFile(String filePath, EFileMode fileMode) = 0;
        virtual const String& GetExecutablePath() const = 0;
    };

}

#endif //BERSERK_SYSTEM_HPP