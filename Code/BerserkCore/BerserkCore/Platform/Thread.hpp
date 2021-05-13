/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_THREAD_HPP
#define BERSERK_THREAD_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Misc/Ref.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringName.hpp>

namespace Berserk {

    /**
     * @brief Thread
     *
     * Represents platform specific OS thread
     */
    class Thread: public RefCounted {
    public:
        using ThreadId = size_t;

        enum class Type {
            Main,
            Secondary
        };

        ~Thread() override = default;

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