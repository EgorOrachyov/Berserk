/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/IResourceImporter.h>
#include <Threading/TSync.h>

namespace Berserk {

    static TSync<TArray<IResourceImporter*>> gImporters;

    void IResourceImporter::registerImporter() {
        TGuard<TArray<IResourceImporter*>> guard(gImporters);
        guard->add(this);
    }

    void IResourceImporter::unregisteImporter() {
        TGuard<TArray<IResourceImporter*>> guard(gImporters);
        guard->removeElement(this);
    }

    TRef<IResourceImporter> IResourceImporter::getResourceFormatImporterFromExt(const CString &extension) {
        TGuard<TArray<IResourceImporter*>> guard(gImporters);
        auto& importers = guard.get();

        for (auto importer: importers) {
            if (importer->getRecognizedExtensions().contains(extension))
                return importer;
        }

        return nullptr;
    }

    TRef<IResourceImporter> IResourceImporter::getResourceFormatImporterFromPath(const CString &path) {
        auto found = path.findLast(".");
        if (!found) return nullptr;
        auto extension = found+1;
        return getResourceFormatImporterFromExt(extension);
    }

}