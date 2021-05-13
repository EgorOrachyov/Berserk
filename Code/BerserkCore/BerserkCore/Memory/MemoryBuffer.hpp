/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORYBUFFER_HPP
#define BERSERK_MEMORYBUFFER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Ref.hpp>

namespace Berserk {

    /** Read-only ref counted memory buffer for safe data passing  */
    class MemoryBuffer: public RefCountedThreadSafe {
    public:
        ~MemoryBuffer() override = default;

        /** @return Pointer to the memory with data  */
        virtual const void* GetData() const = 0;

        /** @return Size of the memory data */
        virtual size_t GetSize() const = 0;

        /** @return True if data ready to be read */
        virtual bool IsDataReady() const = 0;
    };

}

#endif //BERSERK_MEMORYBUFFER_HPP