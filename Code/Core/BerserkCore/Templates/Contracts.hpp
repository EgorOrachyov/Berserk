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

#ifndef BERSERK_CONTRACTS_HPP
#define BERSERK_CONTRACTS_HPP

namespace Berserk {

    template <typename T>
    class Equals;

    template <typename T>
    class Hash;

    template <typename T>
    class CompareLess;

    /**
     * Contract type, used to print arbitrary objects to text format.
     *
     * Implement method template<typename Stream> void operator()(Stream& stream, const T& object) const
     * in order to provide functionality to print your object into arbitrary stream of text symbols.
     *
     * @tparam Stream Generic stream, used to build result text string
     * @tparam T Type of the object to print
     */
    template <typename T>
    class TextPrint;

    // Defaults

    template<typename T>
    class Equals {
    public:
        bool operator()(const T& a, const T& b) const {
            return a == b;
        }
    };

}

#endif //BERSERK_CONTRACTS_HPP
