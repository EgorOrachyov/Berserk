/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXDIALOGS_HPP
#define BERSERK_UNIXDIALOGS_HPP

#include <BerserkCore/Platform/Dialogs.hpp>

namespace Berserk {
    namespace Platform {

        class UnixDialogs: public Dialogs {
        public:

            class UnixImpl final : public Dialogs::Impl {
            public:
                UnixImpl();
                ~UnixImpl() override;
            };

        };

    }
}

#endif //BERSERK_UNIXDIALOGS_HPP
