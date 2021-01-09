/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FILESYSTEM_HPP
#define BERSERK_FILESYSTEM_HPP

#include <BerserkCore/Defines.hpp>
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
            static Ref<File> OpenFile(const String& filepath, File::Mode mode) {
                return Impl::Instance().OpenFile(filepath, mode);
            }

            /**
             * Open native OS file select dialog.
             * Blocks current thread until user selects files and confirms his/her choice.
             *
             * @param title Title of the dialog to set
             * @param defaultPath Path of directory, where open dialog
             * @param patterns Patterns of files to show user in form { "*.jpg", "*.png" } etc.
             * @param paths Returned selected files by the user
             */
            static void OpenFileDialog(const String& title, const String& defaultPath, const Array<String> &patterns, Array<String> &paths) {
                Impl::Instance().OpenFileDialog(title, defaultPath, patterns, paths);
            }

            /**
             * Open native OS folder select dialog.
             * Blocks current thread until user selects folder and confirms his/her choice.
             *
             * @param title Title of the dialog to set
             * @param defaultPath Path of directory, where open dialog
             * @param folderPath Selected folder path
             *
             * @return True if user selected something
             */
            static bool OpenFolderDialog(const String& title, const String& defaultPath, String &folderPath) {
                return Impl::Instance().OpenFolderDialog(title, defaultPath, folderPath);
            }

            /**
             * Open native OS file save dialog.
             * Blocks current thread until user selects folder and confirms his/her choice.
             *
             * @param title Title of the dialog to set
             * @param defaultPath Path of directory, where open dialog
             * @param defaultName Default save file name
             * @param patterns Patterns of files to show user in form { "*.jpg", "*.png" } etc.
             * @param filePath Selected file path
             *
             * @return True if user selected something
             */
            static bool OpenSaveDialog(const String& title, const String& defaultPath, const String& defaultName, const Array<String> &patterns, String& filePath) {
                return Impl::Instance().OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);
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
                String builder;
                MakePathImpl<void, TArgs...>::Make(builder, std::forward<TArgs>(args)...);
                return builder;
            }

        protected:

            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() = default;
                virtual const String& GetExecutablePath() = 0;
                virtual Ref<File> OpenFile(const String& filepath, File::Mode mode) = 0;
                virtual void OpenFileDialog(const String& title, const String& defaultPath, const Array<String> &patterns, Array<String> &paths);
                virtual bool OpenFolderDialog(const String& title, const String& defaultPath, String &folderPath);
                virtual bool OpenSaveDialog(const String& title, const String& defaultPath, const String& defaultName, const Array<String> &patterns, String& filePath);

            protected:
                Mutex mMutex;
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
}

#endif //BERSERK_FILESYSTEM_HPP