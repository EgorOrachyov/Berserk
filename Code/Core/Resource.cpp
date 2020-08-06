/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resource.h>

namespace Berserk {

    // todo: sync with resource cache

    void Resource::setUUID(const UUID& uuid) {
        mUUID = uuid;
    }

    void Resource::setName(Berserk::CString name) {
        mName = std::move(name);
    }

    void Resource::setPath(Berserk::CString path) {
        mPath = std::move(path);
    }

    void Resource::setImportPath(Berserk::CString importPath) {
        mImportPath = std::move(importPath);
    }

    void Resource::setImportTime(const Berserk::TimeValue &importTime) {
        mImportTime = importTime;
    }

    void Resource::showDebugInfo() {
        printf("Resource Info:\n");
        printf(" Type: Resource\n");
        printf(" Name: %s\n", mName.data());
        printf(" UUID: %s\n", mUUID.toString().data());
        printf(" Path: %s\n", mPath.data());
        printf(" Import path: %s\n", mImportPath.data());
        printf(" Import time: %s\n", mImportTime.toFormatString().data());
    }

}