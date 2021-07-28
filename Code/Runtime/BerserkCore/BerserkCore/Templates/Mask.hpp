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

#ifndef BERSERK_MASK_HPP
#define BERSERK_MASK_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    template<typename Enum, typename StoreType = uint32>
    class Mask {
    public:
        Mask() = default;

        Mask(const std::initializer_list<Enum> &bits) {
            for (auto bit: bits) {
                auto offset = (StoreType) bit;
                mMask |= ((StoreType) 1) << offset;
            }
        }

        ~Mask() = default;

        Mask& Set(Enum bit) {
            auto offset = (StoreType) bit;
            mMask |= ((StoreType) 1) << offset;
            return *this;
        }

        Mask& Remove(Enum bit) {
            auto offset = (StoreType) bit;
            auto mask = (StoreType)(((StoreType) 1) << offset);
            mMask &= (~mask);
            return *this;
        }

        bool Get(Enum bit) const {
            auto offset = (StoreType) bit;
            return (mMask & (((StoreType) 1) << offset)) != 0;
        }

        void Reset() {
            mMask = 0;
        }

        StoreType GetMask() const { return mMask; }

    private:
        StoreType mMask = 0;
    };

}

#endif //BERSERK_MASK_HPP