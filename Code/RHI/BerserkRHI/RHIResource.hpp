/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCE_HPP
#define BERSERK_RHIRESOURCE_HPP

#include <BerserkCore/Templates/RefCounted.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief RHI resource
         *
         * Base class for any RHI resource, created by driver and exposed to the user.
         * Uses automated reference counting, required for safe resource life-time
         * tracking and passing among several system threads.
         */
        class Resource: public RefCountedThreadSafe {
        public:
            ~Resource() override = default;
        };

    }
}

#endif //BERSERK_RHIRESOURCE_HPP