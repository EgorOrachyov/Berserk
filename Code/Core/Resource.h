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

        void setUUID(const UUID& uuid);
        void setName(CString name);
        void setPath(CString path);
        void setImportPath(CString importPath);
        void setImportTime(const TimeValue& importTime);

        const UUID& getUUID() const { return mUUID; }
        const CString& getName() const { return mName; }
        const CString& getPath() const { return mPath; }
        const CString& getImportPath() const { return mImportPath; }
        const TimeValue& getImportTime() const { return mImportTime; }

    protected:
        UUID mUUID;
        CString mName;
        CString mPath;
        CString mImportPath;
        TimeValue mImportTime;
    };

}

#endif //BERSERK_RESOURCE_H