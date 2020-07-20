/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Directory.h>

namespace Berserk {
    const char* Directory::typeToString(Berserk::EDirEntryType type) {
        switch (type) {
            case EDirEntryType::File:
                return "File";
            case EDirEntryType::Directory:
                return "Directory";
            case EDirEntryType::Link:
                return "Link";
            default:
                return "Undefined";
        }
    }
}