/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Platform/System.hpp>

namespace Berserk {
    class Log & Debug::GetDebugLog() {
        return Platform::System::Logger();
    }
}
