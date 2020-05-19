/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Engine.h>
#include <IModule.h>

namespace Berserk {

    void IModule::registerModule() {
        Engine::getSingleton().registerModule(this);
    }

    void IModule::unregisterModule() {
        Engine::getSingleton().unregisterModule(this);
    }

}