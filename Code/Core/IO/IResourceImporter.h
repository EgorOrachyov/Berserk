/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IRESOURCEIMPORTER_H
#define BERSERK_IRESOURCEIMPORTER_H

#include <IO/IResourceImportOptions.h>
#include <Resource.h>
#include <IModule.h>
#include <Containers/TArray.h>
#include <Error.h>
#include <TRef.h>

namespace Berserk {

    /**
     * @brief Resource importer interface
     *
     * Resource importer handles importing of external resources into the engine.
     * It is not preferred for dynamic resource importing. Instead is better to use
     * resource loader for fast preprocessed resources loading.
     */
    class IResourceImporter {
    public:
        /**
         * Import resource via specified path
         * @param resource[out] Reference of the loaded resource
         * @param importPath[in] Fully qualified OS path to load resource
         * @param options[in] Import options specific for the resource (by default nullptr)
         * @return Ok if resource successfully loaded, otherwise some error
         */
        virtual EError import(TPtrShared<Resource> &resource, const CString& importPath, const TPtrShared<IResourceImportOptions> &options) = 0;

        /** @return Extensions of the files which could be imported */
        virtual const TArray<CString> &getRecognizedExtensions() const = 0;

        /** @return True, if this importer could be run in several threads */
        virtual bool threadSafe() const { return false; }

        /** @return Resource importer name */
        virtual const char* getImporterName() const = 0;

        /** @return Resource importer project name (where importer was declared) */
        virtual const char* getImporterProjectName() = 0;

        /** @return Finds importer for specified extension */
        static TRef<IResourceImporter> getResourceFormatImporterFromExt(const CString &extension);

        /** @return Finds importer for specified file path */
        static TRef<IResourceImporter> getResourceFormatImporterFromPath(const CString &path);

    protected:

        /** Registers this importer in global engine importer entry */
        void registerImporter();

        /** Unregister this importer in global engine importer entry */
        void unregisteImporter();
    };

}

#endif //BERSERK_IRESOURCEIMPORTER_H