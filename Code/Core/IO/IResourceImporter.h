/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IRESOURCEIMPORTER_H
#define BERSERK_IRESOURCEIMPORTER_H

#include <Resource.h>
#include <TArray.h>
#include <Error.h>

namespace Berserk {

    /**
     * Resource importer handles importing of external resources into the engine.
     * It is not preferred for dynamic resource importing. Instead is better to use
     * resource loader for fast preprocessed resources loading.
     */
    class IResourceImporter {
    public:
        /**
         * Import resource via specified path
         * @param resource[out] Reference of the loaded resource
         * @param importPath Fully qualified OS path to load resource
         * @return Ok if resource successfully loaded, otherwise some error
         */
        virtual EError import(TPtrShared<Resource> &resource, const CString& importPath) = 0;
        /** @return Extensions of the files which could be imported */
        virtual const TArray<CString> &getRecognizedExtensions() const = 0;
    };

}

#endif //BERSERK_IRESOURCEIMPORTER_H