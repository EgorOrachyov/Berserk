//
// Created by Egor Orachyov on 2019-10-08.
//

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
#if DEBUG || EDITOR
            if (!mIsPresent) {
                dev_error("Variant value is not present");
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
