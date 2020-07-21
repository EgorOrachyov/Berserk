/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LIBRARY_H
#define BERSERK_LIBRARY_H

#include <Error.h>
#include <String/CString.h>

namespace Berserk {

    /** Interface for platform specific dynamic/shared library object */
    class Library {
    public:

        virtual ~Library() = default;

        /**
         * Load symbol from library by its name
         * @param symbol Name of the symbol to load
         * @param handle Pointer to save symbol address
         * @return Ok if symbol successfully loaded
         */
        virtual EError loadSymbol(const CString& symbol, void* &handle) = 0;

        /** Close library. After this operation no symbol can be loaded */
        virtual EError close() = 0;

        /** @return True if library is opened */
        virtual bool isOpen() const = 0;

        /** @return Path string used to open this library */
        virtual const CString& loadPath() const = 0;
    };

}

#endif //BERSERK_LIBRARY_H