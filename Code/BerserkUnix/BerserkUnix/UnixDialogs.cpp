/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixDialogs.hpp>

namespace Berserk {
    namespace Platform {

        UnixDialogs::UnixImpl::UnixImpl() {
            Provide(this);
        }

        UnixDialogs::UnixImpl::~UnixImpl() {
            Remove(this);
        }

    }
}