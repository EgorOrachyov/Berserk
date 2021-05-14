/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Platform/DateTime.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/IO/Log.hpp>
#include <BerserkCore/IO/TextWriter.hpp>

namespace Berserk {

    template <typename T>
    class SharedRef;
    template <typename T>
    class SharedPtr;
    template <typename T>
    class WeakPtr;

    class ReferenceControllerOps;
    class Application;

    /**
     * @brief System specifics
     *
     * Provides access to the platform-specific low-level functionality,
     * such as time, date info, cmd args, locale and etc.
     */
    class System {
    public:

        /**
         * Query system date info
         * @param type Specifies whether must interpret as local or global time
         * @return Date info
         */
        static Date GetDate(TimeType type = TimeType::Local) {
            return Impl::Instance().GetDate(type);
        }

        /**
         * Query system time info
         * @param type Specifies whether must interpret as local or global time
         * @return Time info
         */
        static Time GetTime(TimeType type = TimeType::Local) {
            return Impl::Instance().GetTime(type);
        }

        /** @return Current date time as number */
        static TimeStamp GetTimeStamp() {
            return Impl::Instance().GetTimeStamp();
        }

        /** @return Date and time from time stamp */
        static void GetDateTime(TimeStamp timeStamp, Date& date, Time& time, TimeType timeType = TimeType::Local) {
            return Impl::Instance().GetDateTime(timeStamp, date, time, timeType);
        }

        /** @return Out text stream for normal console output */
        static TextWriter& Out() {
            return Impl::Instance().GetOutStream();
        }

        /** @return Error text stream for console output */
        static TextWriter& Error() {
            return Impl::Instance().GetErrorStream();
        }

        /** @return Global engine logger instance */
        static Log& Logger() {
            return Impl::Instance().GetLogger();
        }

        /** Abort application execution */
        static void Abort() {
            Impl::Instance().Abort();
        }

        /** @return Cmg args, passed to the system on start-up */
        static const Array<String>& GetCmdArgs() {
            return Impl::Instance().GetCmdArgs();
        }

        /** @return Locale, set as the global */
        static const String& GetLocale() {
            return Impl::Instance().GetLocale();
        }

        static bool IsStdoutEnabled() {
            return Impl::Instance().IsStdoutEnabled();
        }

        static bool IsStderrEnabled() {
            return Impl::Instance().IsStderrEnabled();
        }

        /** @return True if application provides GUI (windows and user input) */
        static bool IsWindowsEnabled() {
            return Impl::Instance().IsWindowsEnabled();
        }

        static bool IsHdpiEnabled() {
            return Impl::Instance().IsHdpiEnabled();
        }

        static bool UseVsync() {
            return Impl::Instance().UseVsync();
        }

    protected:

        template <typename T>
        friend class ::Berserk::SharedRef;
        template <typename T>
        friend class ::Berserk::SharedPtr;
        template <typename T>
        friend class ::Berserk::WeakPtr;

        friend class ::Berserk::ReferenceControllerOps;
        friend class ::Berserk::String;
        friend class ::Berserk::Application;
        friend class ::Berserk::Memory;

        /** Platform specific implementation of the low-level OS functions */
        class Impl: public Singleton<Impl> {
        public:
            virtual ~Impl() noexcept = default;

            /* General purpose memory management */
            virtual void* Allocate(size_t sizeInBytes) = 0;
            virtual void* Reallocate(void* memory, size_t sizeInBytes) = 0;
            virtual void Deallocate(void* memory) = 0;

            /* String buffers memory management */
            virtual void* AllocateStringBuffer(size_t sizeInBytes) = 0;
            virtual void DeallocateStringBuffer(void *buffer, size_t sizeInBytes) = 0;

            /* Shared pointer meta info buffer memory management */
            virtual void* AllocatePtrMeta(size_t sizeInBytes) = 0;
            virtual void DeallocatePtrMeta(void* buffer) = 0;

            virtual Date GetDate(TimeType type) = 0;
            virtual Time GetTime(TimeType type) = 0;
            virtual TimeStamp GetTimeStamp() = 0;
            virtual void GetDateTime(TimeStamp timeStamp, Date& date, Time& time, TimeType timeType) = 0;

            virtual uint64 GetAllocateCallsCount() const;
            virtual uint64 GetDeallocateCallsCount() const;
            virtual const Array<String>& GetCmdArgs() const;
            virtual const String& GetLocale() const;
            virtual bool IsStdoutEnabled() const;
            virtual bool IsStderrEnabled() const;
            virtual bool IsWindowsEnabled() const;
            virtual bool IsHdpiEnabled() const;
            virtual bool UseVsync() const;

            virtual TextWriter& GetOutStream() = 0;
            virtual TextWriter& GetErrorStream() = 0;
            virtual Log& GetLogger() = 0;

            virtual void Abort() = 0;
            virtual void FixedUpdate() = 0;

        protected:
            AtomicUint64 mAllocCalls{0};
            AtomicUint64 mDeallocCalls{0};
            Array<String> mCmdLine;
            String mLocale;
            bool mHdpiEnabled = false;
            bool mStdoutEnabled = true;
            bool mStderrEnabled = true;
            bool mWindowsEnabled = true;
            bool mUseVsync = true;
        };

    };
}

#endif //BERSERK_SYSTEM_HPP