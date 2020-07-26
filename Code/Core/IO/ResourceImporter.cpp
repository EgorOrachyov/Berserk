/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/ResourceImporter.h>
#include <ResourceImporters.h>

namespace Berserk {

    void ResourceImporter::registerImporter() {
        auto& importers = ResourceImporters::getSingleton();
        importers.registerImporter(*this);
    }

    void ResourceImporter::unregisteImporter() {
        auto& importers = ResourceImporters::getSingleton();
        importers.unregisterImporter(*this);
    }

}