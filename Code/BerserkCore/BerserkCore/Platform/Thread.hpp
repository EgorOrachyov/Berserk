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

#ifndef BERSERK_THREAD_HPP
#define BERSERK_THREAD_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringName.hpp>

namespace Berserk {

    /**
     * @brief Thread
     *
     * Represents platform specific OS thread
     */
    class Thread {
    public:
        using ThreadId = size_t;

        enum class Type {
            Main,
            Secondary
        };

        virtual ~Thread() = default;

        /** Attempts to join this thread with current thread */
        virtual void Join() = 0;

        /** @return True, if can join this thread */
        virtual bool CanJoin() const = 0;

        /** @return True if thread already finished its runnable task */
        virtual bool IsFinished() const = 0;

        /** @return Thread type */
        virtual Type GetType() const = 0;

        /** @return Managed unique thread id */
        virtual ThreadId GetManagedId() const = 0;

        /** @return Friendly name */
        virtual StringName GetName() const = 0;

    };

}

#endif //BERSERK_THREAD_HPP