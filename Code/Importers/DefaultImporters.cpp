/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <DefaultImporters.h>
#include <ImageImporter/ImageImporter.h>
#include <FontImporter/FontImporter.h>
#include <MeshImporter/MeshImporter.h>

namespace Berserk {

    DefaultImporters::DefaultImporters() {
        imageImporter = TPtrUnique<ImageImporter>::make();
        fontImporter = TPtrUnique<FontImporter>::make();
        meshImporter = TPtrUnique<MeshImporter>::make();
    }

    DefaultImporters::~DefaultImporters() {
        // keep empty
    }

}