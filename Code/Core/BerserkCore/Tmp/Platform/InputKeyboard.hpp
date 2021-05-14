/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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