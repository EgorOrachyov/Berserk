/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RESOURCEIMPORTERS_H
#define BERSERK_RESOURCEIMPORTERS_H

#include <TRef.h>
#include <Containers/TArray.h>
#include <IO/ResourceImporter.h>

namespace Berserk {

    /** Manage all the available resource importers at runtime */
    class ResourceImporters {
    public:

        ResourceImporters();
        ~ResourceImporters();

        /**
         * Attempts to find importer from extension in specified resource path
         * @param pathToResource Path to the resource file
         * @return Not null resource importer
         */
        TRef<ResourceImporter> findImporterFromPath(const CString &pathToResource);

        /**
         * Attempts to find importer from extension
         * @param extension Resource extension
         * @return Not null resource importer
         */
        TRef<ResourceImporter> findImporterFromExtension(const CString &extension);

        /** Register importer in system */
        void registerImporter(ResourceImporter &importer);

        /** Unregister importer in system */
        void unregisterImporter(ResourceImporter &importer);

        /** @return Engine resources importers managers */
        static ResourceImporters& getSingleton();

    private:

        Mutex mMutex;
        TArray<ResourceImporter*> mImporters;

        static ResourceImporters* gResourceImporters;

    };

}

#endif //BERSERK_RESOURCEIMPORTERS_H