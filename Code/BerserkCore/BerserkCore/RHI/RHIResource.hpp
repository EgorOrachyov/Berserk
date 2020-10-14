/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCE_HPP
#define BERSERK_RHIRESOURCE_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    class RHIResource {
    public:
        virtual ~RHIResource() = default;
    };

}

#endif //BERSERK_RHIRESOURCE_HPP