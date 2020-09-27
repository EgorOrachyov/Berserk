/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Printf.hpp>
#include <BerserkCore/String/TStringUtils.hpp>

namespace Berserk {

    void Printf::Add(const char *string) {
        uint32 length = StringUtils::Length(string);
        mBuffer.Add(string, length);
    }

    void Printf::Add(Berserk::int32 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%i", i);
        mBuffer.Add(buffer, written);
    }

    void Printf::Add(Berserk::uint32 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%u", i);
        mBuffer.Add(buffer, written);
    }

    void Printf::Add(Berserk::int64 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%lli", i);
        mBuffer.Add(buffer, written);
    }

    void Printf::Add(Berserk::uint64 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%llu", i);
        mBuffer.Add(buffer, written);
    }

    void Printf::Add(float f) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%f", f);
        mBuffer.Add(buffer, written);
    }

    void Printf::Add(double f) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%lf", f);
        mBuffer.Add(buffer, written);
    }

    void Printf::Addnl() {
        char buffer[STACK_BUFFER] = "\n";
        mBuffer.Add(buffer, 1);
    }

}