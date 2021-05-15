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

#ifndef BERSERK_TMASK_HPP
#define BERSERK_TMASK_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <initializer_list>

namespace Berserk {

    template <typename T, typename S = uint32>
    class TMask {
    public:
        TMask() = default;
        TMask(const TMask& other) = default;
        TMask(TMask&& other) noexcept = default;
        ~TMask() = default;

        TMask(const std::initializer_list<T>& bits) { for (auto bit: bits) { IntSetBit(bit); } }

        TMask& operator=(const TMask& other) = default;
        TMask& operator=(TMask&& other) noexcept = default;

        bool operator==(const TMask& other) const { return mStore == other.mStore; }
        bool operator!=(const TMask& other) const { return mStore != other.mStore; }

        TMask operator&(const TMask& other) const { TMask r; r.mStore = mStore & other.mStore; return r; }
        TMask operator|(const TMask& other) const { TMask r; r.mStore = mStore | other.mStore; return r; }
        TMask operator^(const TMask& other) const { TMask r; r.mStore = mStore ^ other.mStore; return r; }

        TMask AddBit(T bit) const { TMask r; r.AddBit(bit); return r; }
        TMask RemoveBit(T bit) const { TMask r; r.IntRemoveBit(bit); return r; }

        bool GetBit(T bit) const { return (mStore & (1u << (uint32)bit)) != 0; };
        S GetValue() const { return mStore; }

    private:
        void IntSetBit(T bit) { mStore |= 1u << (uint32)bit; }
        void IntRemoveBit(T bit) { mStore &= (~0u) ^ (1u << (uint32)bit); }

        S mStore = 0;
    };

}

#endif //BERSERK_TMASK_HPP