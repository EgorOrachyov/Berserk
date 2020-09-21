/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PRINTF_HPP
#define BERSERK_PRINTF_HPP

#include <BerserkCore/Containers/TArray.hpp>

namespace Berserk {

    class Printf {
    public:
        void add(const char* string);
        void add(int32 i);
        void add(uint32 i);
        void add(int64 i);
        void add(uint64 i);
        void add(float f);
        void add(double f);
        void addnl();

        Printf& operator<<(const char* string) { add(string); return *this; }
        Printf& operator<<(int32 i) { add(i); return *this; }
        Printf& operator<<(uint32 i) { add(i); return *this; }
        Printf& operator<<(int64 i) { add(i); return *this; }
        Printf& operator<<(uint64 i) { add(i); return *this; }
        Printf& operator<<(float f) { add(f); return *this; }
        Printf& operator<<(double f) { add(f); return *this; }

        uint32 getBufferSize() const { return mBuffer.getSize(); }
        const char* getBuffer() const { return mBuffer.getCBuffer(); }

    private:
        static const uint32 STACK_BUFFER = 64;
        TArray<char> mBuffer;
    };

}

#endif //BERSERK_PRINTF_HPP