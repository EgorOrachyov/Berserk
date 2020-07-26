/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ResourceImporters.h>

namespace Berserk {

    ResourceImporters* ResourceImporters::gResourceImporters = nullptr;

    ResourceImporters::ResourceImporters() {
        if (gResourceImporters) {
            fprintf(stderr, "[BERSERK Core] Only single ResourceImporters could be set as singleton");
            return;
        }

        gResourceImporters = this;
    }

    ResourceImporters::~ResourceImporters() {
        BERSERK_COND_ERROR(mImporters.size() == 0, "All importers must be unregistered");

        gResourceImporters = nullptr;
    }
    
    void ResourceImporters::registerImporter(ResourceImporter &importer) {
        Guard guard(mMutex);

        auto ptr = &importer;
        BERSERK_COND_ERROR_RET(!mImporters.contains(ptr), "Importer already registered: %s", importer.getImporterName());
        mImporters.add(ptr);
    }
    
    void ResourceImporters::unregisterImporter(ResourceImporter &importer) {
        Guard guard(mMutex);

        auto ptr = &importer;
        BERSERK_COND_ERROR_RET(mImporters.contains(ptr), "Importer was not registered: %s", importer.getImporterName());
        mImporters.removeElement(&importer);
    }

    TRef<ResourceImporter> ResourceImporters::findImporterFromPath(const CString &pathToResource) {
        auto ext = pathToResource.findLast(".");
        BERSERK_COND_ERROR_RET_VALUE(nullptr, ext != nullptr, "Resource path has no extension: %s", pathToResource.data());
        auto extStr = CString{ext+1};
        return findImporterFromExtension(extStr);
    }

    TRef<ResourceImporter> ResourceImporters::findImporterFromExtension(const Berserk::CString &extension) {
        Guard guard(mMutex);

        for (auto importer: mImporters) {
            if (importer->getRecognizedExtensions().contains(extension))
                return importer;
        }

        return nullptr;
    }

    ResourceImporters& ResourceImporters::getSingleton() {
        return *gResourceImporters;
    }

}