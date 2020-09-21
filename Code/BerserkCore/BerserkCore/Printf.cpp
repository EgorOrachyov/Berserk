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

    void Printf::add(const char *string) {
        uint32 length = StringUtils::length(string);
        mBuffer.add(string, length);
    }

    void Printf::add(Berserk::int32 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%i", i);
        mBuffer.add(buffer, written);
    }

    void Printf::add(Berserk::uint32 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%u", i);
        mBuffer.add(buffer, written);
    }

    void Printf::add(Berserk::int64 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%lli", i);
        mBuffer.add(buffer, written);
    }

    void Printf::add(Berserk::uint64 i) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%llu", i);
        mBuffer.add(buffer, written);
    }

    void Printf::add(float f) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%f", f);
        mBuffer.add(buffer, written);
    }

    void Printf::add(double f) {
        char buffer[STACK_BUFFER];
        uint32 written = snprintf(buffer, STACK_BUFFER, "%lf", f);
        mBuffer.add(buffer, written);
    }

    void Printf::addnl() {
        char buffer[STACK_BUFFER] = "\n";
        mBuffer.add(buffer, 1);
    }

}