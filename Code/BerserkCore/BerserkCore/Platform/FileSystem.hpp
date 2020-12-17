/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FILESYSTEM_HPP
#define BERSERK_FILESYSTEM_HPP

#include <BerserkCore/BuildOptions.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/Misc/PtrShared.hpp>
#include <BerserkCore/Strings/StringBuilder.hpp>

namespace Berserk {
    namespace Platform {

        /**
         * @brief File System
         *
         * Provides platform specific access for files, paths and directories manipulations.
         */
        class FileSystem {
        public:

            enum class PathType {
                Unix,
                Windows,

            #ifdef BERSERK_TARGET_WINDOWS
                Current = Windows
            #elif defined(BERSERK_TARGET_LINUX) || defined(BERSERK_TARGET_MACOS)
                Current = Unix
            #else
                #error Unsupported file system path type
            #endif
            };

            static const char UnixFileSeparator = '/';
            static const char WindowsFileSeparator = '\\';
            static const char FileSeparator = PathType::Current == PathType::Unix? UnixFileSeparator: WindowsFileSeparator;

            /**
             * Open or create file with specified options.
             *
             * @param filepath Relative or full path to the file
             * @param mode Open mode of the file
             *
             * @return Handler to the opened file
             */
            static PtrShared<File> OpenFile(const String& filepath, File::Mode mode) {
                return Impl::Instance().OpenFile(filepath, mode);
            }

            /**
             * Query absolute path of the this application executable file.
             *
             * @return Path to the executable.
             */
            static const String& GetExecutablePath() {
                return Impl::Instance().GetExecutablePath();
            }

            /** @return File name for current platform file type */
            static String GetFileNameFromPath(const String& path);

            /** @return File name from unix based path */
            static String GetFileNameFromPathUnix(const String& path);

            /** @return File name from windows based path */
            static String GetFileNameFromPathWindows(const String& path);

            /**
             * Make current platform specific path sequence
             *
             * @tparam TArgs Types of objects, which represent entries names
             * @param args Names of entries to concatenate
             *
             * @return Current platform path for entries
             */
            template<typename ... TArgs>
            static String MakePath(TArgs&& ... args) {
                StringBuilder builder;
                MakePathImpl<void, TArgs...>::Make(builder, std::forward<TArgs>(args)...);
                return builder.ToString();
            }

        protected:

            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() = default;
                virtual const String& GetExecutablePath() = 0;
                virtual PtrShared<File> OpenFile(const String& filepath, File::Mode mode) = 0;
            };

            template<typename D, typename ... TArgs>
            struct MakePathImpl { };

            template<typename D, typename T>
            struct MakePathImpl<D, T> {
                static void Make(StringBuilder& builder, T&& arg) {
                    builder.Add(arg);
                }
            };

            template<typename D, typename T, typename ... TArgs>
            struct MakePathImpl<D, T, TArgs...> {
                static void Make(StringBuilder& builder, T&& arg, TArgs&& ... args) {
                    builder.Add(arg);
                    builder.Add(FileSeparator);
                    MakePathImpl<D, TArgs...>::Make(builder, std::forward<TArgs>(args)...);
                }
            };

        };

    }
}

#endif //BERSERK_FILESYSTEM_HPP