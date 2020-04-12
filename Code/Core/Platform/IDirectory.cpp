/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/IDirectory.h>

namespace Berserk {
    const char* IDirectory::typeToString(Berserk::EDirEntryType type) {
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