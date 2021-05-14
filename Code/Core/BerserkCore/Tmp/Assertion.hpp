/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ASSERTION_HPP
#define BERSERK_ASSERTION_HPP

#include <BerserkCore/Platform/Platform.hpp>

#define BERSERK_ASSERT(condition) \
    if (!(condition)) { Details::AssertionOnFail(#condition, __LINE__, __FUNCTION__, __FILE__); }

#define BERSERK_ASSERT_MSG(condition, message) \
    if (!(condition)) { Details::AssertionOnFail(#condition, message, __LINE__, __FUNCTION__, __FILE__); }

namespace Berserk {
    namespace Details {
        void AssertionOnFail(const char* what, uint64 line, const char* function, const char* file);
        void AssertionOnFail(const char* what, const char* msg, uint64 line, const char* function, const char* file);
    }
}

#endif //BERSERK_ASSERTION_HPP
