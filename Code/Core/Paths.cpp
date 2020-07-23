/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Paths.h>
#include <Platform/System.h>

namespace Berserk {

    // Maybe rewrite later
    // Fix names of the directories and constants

    System& getSystemRef() {
        static System& cached = System::getSingleton();
        return cached;
    }

    CString Paths::getRootDirectory() {
        return getSystemRef().getRootPath();
    }

    CString Paths::getEngineDirectory() {
        return getSystemRef().getRootPath() + "/Engine";
    }

    CString Paths::getEditorDirectory() {
        return getSystemRef().getRootPath() + "/Editor";
    }

    CString Paths::getFullPathFor(const Berserk::CString &relativePath, Berserk::EPathType pathType) {
        switch (pathType) {
            case EPathType::Root:
                return getRootDirectory() + "/" + relativePath;
            case EPathType::Engine:
                return getEngineDirectory() + "/" + relativePath;
            case EPathType::Editor:
                return getEditorDirectory() + "/" + relativePath;
            default:
                return relativePath;
        }
    }

}