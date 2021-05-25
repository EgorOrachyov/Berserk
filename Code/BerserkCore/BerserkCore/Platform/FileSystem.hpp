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

#ifndef BERSERK_FILESYSTEM_HPP
#define BERSERK_FILESYSTEM_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>
#include <BerserkCore/Strings/StringBuilder.hpp>

namespace Berserk {

    /**
     * @brief File System
     *
     * Provides platform specific access for files, paths and directories manipulations.
     */
    class FileSystem {
    public:

        enum class PathType {
            Unix = BERSERK_PATH_TYPE_UNIX,
            Windows = BERSERK_PATH_TYPE_WINDOWS,
            Current = BERSERK_PATH_TYPE
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
        static SharedPtr<File> OpenFile(const String& filepath, File::Mode mode) {
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

        /** @return File extension (if present) without '.' symbol */
        static String GetFileExtension(const String& filename);

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
            String builder;
            MakePathImpl<void, TArgs...>::Make(builder, std::forward<TArgs>(args)...);
            return builder;
        }

    protected:

        class Impl: public Singleton<Impl> {
        public:
            virtual ~Impl() = default;
            virtual const String& GetExecutablePath() = 0;
            virtual SharedPtr<File> OpenFile(const String& filepath, File::Mode mode) = 0;
        };

        template<typename D, typename ... TArgs>
        struct MakePathImpl { };

        template<typename D, typename T>
        struct MakePathImpl<D, T> {
            template<typename Builder>
            static void Make(Builder& builder, T&& arg) {
                builder.Add(arg);
            }
        };

        template<typename D, typename T, typename ... TArgs>
        struct MakePathImpl<D, T, TArgs...> {
            template<typename Builder>
            static void Make(Builder& builder, T&& arg, TArgs&& ... args) {
                builder.Add(arg);
                builder.Add(FileSeparator);
                MakePathImpl<D, TArgs...>::Make(builder, std::forward<TArgs>(args)...);
            }
        };

    };
}

#endif //BERSERK_FILESYSTEM_HPP