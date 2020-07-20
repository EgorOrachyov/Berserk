/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Engine.h>
#include <Module.h>

namespace Berserk {

    void Module::registerModule() {
        Engine::getSingleton().registerModule(this);
    }

    void Module::unregisterModule() {
        Engine::getSingleton().unregisterModule(this);
    }

}
