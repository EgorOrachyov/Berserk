/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    String DateTime::ToString() const {
        char buffer[64];

        sprintf(buffer, "%i.%i.%i %i:%i:%i",
                year, month+1, dayMonth+1, hour, min, sec);

        return buffer;
    }

    String DateTime::ToStringConservative() const {
        char buffer[64];

        sprintf(buffer, "%i-%i-%i_%i-%i-%i",
                year, month+1, dayMonth+1, hour, min, sec);

        return buffer;
    }

}