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
#include <BerserkCore/Strings/String.hpp>
#include <exception>

namespace Berserk {

    /**
     * @brief Exception
     *
     * Base class for any Engine exception.
     */
    class Exception : public std::exception {
    public:

        Exception(String typeName, String description, String source, String file, size_t line) noexcept
                : mTypeName(std::move(typeName)), mDescription(std::move(description)), mSource(std::move(source)),
                  mFile(std::move(file)), mLine(line) {

        }

        Exception(const Exception& exception)
            : mTypeName(exception.mTypeName), mDescription(exception.mDescription),
              mSource(exception.mSource), mFile(exception.mFile), mLine(exception.mLine) {

            }

        ~Exception() noexcept override = default;

        Exception& operator=(const Exception& other) {
            mTypeName = other.mTypeName;
            mDescription = other.mDescription;
            mSource = other.mSource;
            mFile = other.mFile;
            mLine = other.mLine;

            return *this;
        }

        /** @return the source function that threw the exception. */
        virtual const String &getSource() const noexcept {
            return mSource;
        }

        /** @return the source file name in which the exception was thrown. */
        virtual const String &getFile() const noexcept {
            return mFile;
        }

        /** @return line number on which the exception was thrown. */
        virtual long getLine() const noexcept {
            return mLine;
        }

        /** @return a short description about the exception. */
        virtual const String &getDescription() const noexcept {
            return mDescription;
        }

        /** @return exception type name */
        virtual const String &getTypeName() const noexcept {
            return mTypeName;
        }

        const char *what() const noexcept override {
            return mDescription.GetStr_C();
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
        AssertionException(String description, String source, String file, size_t line) noexcept
            : Exception("AssertionException", std::move(description), std::move(source), std::move(file), line) {

        }
    };

    class InvalidArgumentException: public Exception {
    public:
        InvalidArgumentException(String description, String source, String file, size_t line) noexcept
                : Exception("InvalidArgumentException", std::move(description), std::move(source), std::move(file), line) {

        }
    };

    class InvalidOpException: public Exception {
    public:
        InvalidOpException(String description, String source, String file, size_t line) noexcept
                : Exception("InvalidOpException", std::move(description), std::move(source), std::move(file), line) {

        }
    };

#define BERSERK_EXCEPT(type, description)                                    \
    {                                                                        \
        throw type (description, __FUNCTION__, __FILE__, (size_t) __LINE__); \
    }

}

#endif //BERSERK_EXCEPTION_HPP