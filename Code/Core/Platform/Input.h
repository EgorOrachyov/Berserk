/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INPUT_H
#define BERSERK_INPUT_H

#include <Math/Point2i.h>
#include <Platform/InputEnums.h>

namespace Berserk {

    /** Handles raw OS input from various devices */
    class Input {
    public:
        Input();
        virtual ~Input() = default;

        virtual Point2i getMousePosition() const = 0;
        virtual Point2i getMouseDelta() const = 0;
        virtual bool isButtonPressed(EMouseButton button) const = 0;
        virtual bool isButtonReleased(EMouseButton button) const = 0;

        virtual EModifiersMask getModifiersMask() const = 0;

        virtual bool isKeyPressed(EKeyboardKey key) const = 0;
        virtual bool isKeyReleased(EKeyboardKey key) const = 0;
        virtual bool isKeyRepeated(EKeyboardKey key) const = 0;

        static Input& getSingleton();
    };

}

#endif //BERSERK_INPUT_H