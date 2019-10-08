//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_TVARIANT_H
#define BERSERK_TVARIANT_H

#include <HAL/Types.h>

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

        TVariant() {

        }

        TVariant(const T& value) {
            new (mBuffer) T(value);
            mIsPresent = true;
        }

        ~TVariant() {
            if (mIsPresent) {

            }
        }

        bool isPresent() const {
            return mIsPresent;
        }

        bool isNull() const {
            return !mIsPresent;
        }

        T& get() const {
            if (mIsPresent) {
                return *((T*)mBuffer);
            } else {
                // todo: throw exception
            }
        }

    private:

        bool mIsPresent = false;
        uint8 mBuffer[sizeof(T)];

    };


}

#endif //BERSERK_TVARIANT_H
