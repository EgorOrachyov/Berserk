/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHITIMEQUERY_H
#define BERSERK_RHITIMEQUERY_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHITimeQuery : public RHIResource {
    public:
        ~RHITimeQuery() override = default;
    };

}

#endif //BERSERK_RHITIMEQUERY_H
