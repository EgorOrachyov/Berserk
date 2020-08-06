/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_DEFAULTIMPORTERS_H
#define BERSERK_DEFAULTIMPORTERS_H

#include <ImageImporter/ImageImporter.h>
#include <FontImporter/FontImporter.h>

namespace Berserk {

    class DefaultImporters {
    public:
        ImageImporter imageImporter;
        FontImporter fontImporter;
    };

}

#endif //BERSERK_DEFAULTIMPORTERS_H
