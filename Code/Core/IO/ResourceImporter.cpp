/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/ResourceImporter.h>
#include <Threading/TSync.h>

namespace Berserk {

    static TSync<TArray<ResourceImporter*>> gImporters;

    void ResourceImporter::registerImporter() {
        TGuard<TArray<ResourceImporter*>> guard(gImporters);
        guard->add(this);
    }

    void ResourceImporter::unregisteImporter() {
        TGuard<TArray<ResourceImporter*>> guard(gImporters);
        guard->removeElement(this);
    }

    TRef<ResourceImporter> ResourceImporter::getResourceFormatImporterFromExt(const CString &extension) {
        TGuard<TArray<ResourceImporter*>> guard(gImporters);
        auto& importers = guard.get();

        for (auto importer: importers) {
            if (importer->getRecognizedExtensions().contains(extension))
                return importer;
        }

        return nullptr;
    }

    TRef<ResourceImporter> ResourceImporter::getResourceFormatImporterFromPath(const CString &path) {
        auto found = path.findLast(".");
        if (!found) return nullptr;
        auto extension = found+1;
        return getResourceFormatImporterFromExt(extension);
    }

}