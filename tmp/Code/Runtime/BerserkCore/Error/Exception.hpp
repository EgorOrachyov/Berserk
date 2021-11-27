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

        Exception(String typeName, String description, String source, String file, uint64 line) noexcept
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
        virtual uint64 getLine() const noexcept {
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
        uint64 mLine;
    };

#define BERSERK_EXCEPTION_TEMPLATE(type)                                                                \
    class type: public Exception {                                                                      \
    public:                                                                                             \
        type(String description, String source, String file, uint64 line) noexcept                      \
                : Exception(#type, std::move(description), std::move(source), std::move(file), line) {  \
    }                                                                                                   \
    };

    BERSERK_EXCEPTION_TEMPLATE(AssertionException)
    BERSERK_EXCEPTION_TEMPLATE(FatalError)
    BERSERK_EXCEPTION_TEMPLATE(InvalidArgumentException)
    BERSERK_EXCEPTION_TEMPLATE(InvalidOpException)

#define BERSERK_EXCEPT(type, description)                                    \
    {                                                                        \
        throw type (description, __FUNCTION__, __FILE__, (uint64) __LINE__); \
    }

}

#endif //BERSERK_EXCEPTION_HPP