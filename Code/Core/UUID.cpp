/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <UUID.h>
#include <Math/Random.h>

namespace Berserk {

    UUID UUID::generate() {
        static Random random;
        UUID result;

        for (auto& v: result.buffer) {
            v = random.randi32();
        }

        return result;
    }

}