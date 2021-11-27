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

#ifndef BERSERK_SINGLETON_HPP
#define BERSERK_SINGLETON_HPP

#include <core/Config.hpp>

BERSERK_NS_BEGIN

/**
 * @addtogroup Core
 * @{
 */

/**
 * @class Singleton
 * @brief Singleton class
 *
 * @tparam TInstance Type of provided singleton
 */
template<typename TInstance>
class BERSERK_API Singleton {
public:
    /** @return Public instance pointer */
    static TInstance *InstancePointer() {
        return mInstancePointer;
    }

    /** @return Public instance reference */
    static TInstance &Instance() {
        return *mInstancePointer;
    }

protected:
    /** Must be called by instance provider */
    static void Provide(TInstance *reference) {
        if (mInstancePointer == nullptr) {
            mInstancePointer = reference;
        }
    }

    /** Must be called by instance provider */
    static void Remove(TInstance *reference) {
        if (mInstancePointer == reference) {
            mInstancePointer = nullptr;
        }
    }

private:
    static TInstance *mInstancePointer;
};

template<typename TInstance>
TInstance *Singleton<TInstance>::mInstancePointer = nullptr;

/**
 * @}
 */

BERSERK_NS_END

#endif//BERSERK_SINGLETON_HPP
