/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_DIRECTORY_H
#define BERSERK_DIRECTORY_H

#include <Typedefs.h>
#include <String/CString.h>
#include <Containers/TArray.h>

namespace Berserk {

    /** Entries type inside a folder */
    enum class EDirEntryType {
        Undefined = 0,
        Directory,
        File,
        Link
    };

    /** Platform directory access */
    class Directory {
    public:
        struct Entry {
        public:
            CString name;
            EDirEntryType type = EDirEntryType::Undefined;
        };
        virtual ~Directory() = default;
        /** Closes directory (after this operation directory is inaccessible) */
        virtual void close() = 0;
        /** @return True, if directory is open */
        virtual bool isOpen() const = 0 ;
        /** @return Directory open path */
        virtual const CString& getPath() const = 0;
        /** @return Directory absolute path */
        virtual const CString& getRealPath() const = 0;
        /** @return Directory entries */
        virtual const TArray<Entry> &getEntries() const = 0;
        /** @return String representation of the dir type */
        static const char* typeToString(EDirEntryType type);
    };

}

#endif //BERSERK_DIRECTORY_H