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
#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/Misc/DateTime.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/IO/Log.hpp>
#include <BerserkCore/IO/TextWriter.hpp>

namespace Berserk {

    template <typename T>
    class PtrShared;
    class Application;

    namespace Platform {

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

            /** @return Cmg args, passed to the system on start-up */
            static const Array<String>& GetCmdArgs() {
                return Impl::Instance().GetCmdArgs();
            }

            /** @return Locale, set as the global */
            static const String& GetLocale() {
                return Impl::Instance().GetLocale();
            }

            /** @return True, if actual console is available in current application */
            static bool IsOutputPresented() {
                return Impl::Instance().IsOutputPresented();
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

            /** @return True if application provides GUI (windows and user input) */
            static bool HasNativeGui() {
                return Impl::Instance().HasNativeGui();
            }

        protected:

            template<typename T>
            friend class ::Berserk::PtrShared;
            friend class ::Berserk::String;
            friend class ::Berserk::Application;
            friend class ::Berserk::Platform::Memory;

            /** Platform specific implementation of the low-level OS functions */
            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() noexcept = default;

                /* General purpose memory management */
                virtual void* Allocate(size_t sizeInBytes) = 0;
                virtual void Deallocate(void* memory) = 0;

                virtual uint64 GetAllocateCallsCount() const = 0;
                virtual uint64 GetDeallocateCallsCount() const = 0;

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

                virtual const Array<String>& GetCmdArgs() const = 0;
                virtual const String& GetLocale() const = 0;

                virtual bool IsOutputPresented() const = 0;
                virtual TextWriter& GetOutStream() = 0;
                virtual TextWriter& GetErrorStream() = 0;
                virtual Log& GetLogger() = 0;

                virtual void Abort() = 0;
                virtual bool HasNativeGui() const = 0;
                //virtual bool IsExitRequested() const = 0;
                virtual void FixedUpdate() = 0;
            };

        };

    }
}

#endif //BERSERK_SYSTEM_HPP