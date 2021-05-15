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

#ifndef BERSERK_INPUTKEYBOARD_HPP
#define BERSERK_INPUTKEYBOARD_HPP

#include <BerserkCore/Platform/InputDefinitions.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/TMask.hpp>

namespace Berserk {

    class InputKeyboard {
    public:
        static const uint32 MAX_KEYS_COUNT = (uint32)EKeyboardKey::MarkerCount;
        virtual ~InputKeyboard() = default;

        virtual const String& GetName() const = 0;
        virtual void GetTextInput(TArray<uint32> &codepoints) = 0;
        virtual EInputAction GetKeyAction(EKeyboardKey key) const = 0;
        virtual EDeviceState GetState() const = 0;
        virtual TMask<EModifier> GetModifiersMask() const = 0;
        virtual bool HasKeyInput() const = 0;
    };

}

#endif //BERSERK_INPUTKEYBOARD_HPP