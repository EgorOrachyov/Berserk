/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

#ifndef BERSERK_TVARIANT_H
#define BERSERK_TVARIANT_H

#include <Misc/Error.h>

namespace Berserk {

    /**
     * Stores object of type T, if it is present,
     * otherwise signals, that it is null.
     *
     * @tparam T Stored internal object
     */
    template <typename T>
    class TVariant {
    public:

        /** Null variant */
        TVariant() = default;

        /** Not null variant from value */
        TVariant(T& value) {
            new (mBuffer) T(value);
            mIsPresent = true;
        }

        /** Destroy variant if present */
        ~TVariant() {
            if (mIsPresent) {
                get().~T();
                mIsPresent = false;
            }
        }

        /** @return True if value present */
        inline bool isPresent() const {
            return mIsPresent;
        }

        /** @return True if value is not present */
        inline bool isNull() const {
            return !mIsPresent;
        }

        /**
         * @throw Exception if variant null
         * @return Value pointer if present
         */
        inline T* operator->() const {
            return get();
        }

        /**
         * @throw Exception if variant null
         * @return Value reference if present
         */
        inline T& get() const {
#if MODE_DEBUG || MODE_EDITOR
            if (!mIsPresent) {
                DEV_ERROR("Variant value is not present");
            }
#endif
            return *((T*)mBuffer);
        }

    private:

        /** True if present */
        bool mIsPresent = false;

        /** Actual T object buffer */
        uint8 mBuffer[sizeof(T)] = { };

    };


}

#endif //BERSERK_TVARIANT_H
