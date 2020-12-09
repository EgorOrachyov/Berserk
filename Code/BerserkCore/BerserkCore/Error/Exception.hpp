/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_EXCEPTION_HPP
#define BERSERK_EXCEPTION_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/String/String.hpp>
#include <exception>

namespace Berserk {
    namespace Error {

        /**
         * @brief Exception
         *
         * Base class for any Engine exception.
         */
        class Exception : public std::exception {
        public:
            using String = String::String;

            Exception(String typeName, String description, String source, String file, size_t line)
                    : mTypeName(std::move(typeName)), mDescription(std::move(description)), mSource(std::move(source)),
                      mFile(std::move(file)), mLine(line) {

            }

            ~Exception() noexcept override = default;

            /** @return the source function that threw the exception. */
            virtual const String &getSource() const {
                return mSource;
            }

            /** @return the source file name in which the exception was thrown. */
            virtual const String &getFile() const {
                return mFile;
            }

            /** @return line number on which the exception was thrown. */
            virtual long getLine() const {
                return mLine;
            }

            /** @return a short description about the exception. */
            virtual const String &getDescription() const {
                return mDescription;
            }

            /** @return exception type name */
            virtual const String &getTypeName() const {
                return mTypeName;
            }

            const char *what() const noexcept override {
                return mDescription.GetStr();
            }

        private:
            String mTypeName;
            String mDescription;
            String mSource;
            String mFile;
            size_t mLine;
        };

        class AssertionException: public Exception {
        public:
            AssertionException(String description, String source, String file, size_t line)
                : Exception("AssertionException", std::move(description), std::move(source), std::move(file), line) {

            }

            ~AssertionException() noexcept override = default;
        };

#define BERSERK_EXCEPT(type, description)                                       \
        {                                                                       \
            using namespace ::Berserk::Error;                                   \
            throw type(description, __FUNCTION__, __FILE__, (size_t) __LINE__); \
        }

    }
}

#endif //BERSERK_EXCEPTION_HPP