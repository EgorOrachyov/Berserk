/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RESOURCE_H
#define BERSERK_RESOURCE_H

#include <Typedefs.h>
#include <String/CString.h>
#include <UUID.h>
#include <TimeValue.h>
#include <TPtrShared.h>
#include <IO/Archive.h>

namespace Berserk {

    /**
     * @brief Resource base class
     *
     * Base class for the any engine resource, data which could be imported, saved or loaded from disk.
     * Each resource has a path in asset folder and unique externally generated UUID.
     */
    class Resource {
    public:
        virtual ~Resource() = default;

        /**
         * For resources which share state with systems, executed possibly in the other thread.
         * @return If the resource fully loaded on the other thread/
         */
        virtual bool isLoaded() const { return false; }

        /** Set resource unique identifier used to find resource in assets library */
        void setUUID(const UUID& uuid);

        /** Set descriptive resource name for debugging */
        void setName(CString name);

        /** Set resource assets path */
        void setPath(CString path);

        /** Set resource import path for debugging */
        void setImportPath(CString importPath);

        /** Set time when resource was imported for debugging */
        void setImportTime(const TimeValue& importTime);

        /** @return Resource unique identifier used to find resource in assets library */
        const UUID& getUUID() const { return mUUID; }

        /** @return Descriptive resource name for debugging */
        const CString& getName() const { return mName; }

        /** @return Resource assets path */
        const CString& getPath() const { return mPath; }

        /** @return Resource import path for debugging */
        const CString& getImportPath() const { return mImportPath; }

        /** @return Time when resource was imported for debugging */
        const TimeValue& getImportTime() const { return mImportTime; }

        /** Shows debug resource info into console */
        virtual void showDebugInfo();

    protected:
        UUID mUUID;
        CString mName;
        CString mPath;
        CString mImportPath;
        TimeValue mImportTime;
    };

}

#endif //BERSERK_RESOURCE_H