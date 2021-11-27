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

#ifndef BERSERK_CONFIG_HPP
#define BERSERK_CONFIG_HPP

/** Macro to specify dll exported classes (runtime api) */
#define BERSERK_API

/** For msvc dynamic build override attribute */
#ifdef BERSERK_DYNAMIC_BUILD
    #ifdef BERSERK_MSVC
        #undef BERSERK_API
        #ifdef BERSERK_EXPORTS
            #define BERSERK_API __declspec(dllexport)
        #else
            #define BERSERK_API __declspec(dllimport)
        #endif//BERSERK_EXPORTS
    #endif    //BERSERK_MSVC
#endif        //BERSERK_DYNAMIC_BUILD

/** Project namespace config */
#define BERSERK_NS_BEGIN namespace berserk {
#define BERSERK_NS_END }
#define BERSERK_NS_USE using namespace berserk;
#define BERSERK_NS ::berserk

#endif//BERSERK_CONFIG_HPP
