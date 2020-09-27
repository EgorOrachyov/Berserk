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
        void Add(const char* string);
        void Add(int32 i);
        void Add(uint32 i);
        void Add(int64 i);
        void Add(uint64 i);
        void Add(float f);
        void Add(double f);
        void Addnl();

        Printf& operator<<(const char* string) { Add(string); return *this; }
        Printf& operator<<(int32 i) { Add(i); return *this; }
        Printf& operator<<(uint32 i) { Add(i); return *this; }
        Printf& operator<<(int64 i) { Add(i); return *this; }
        Printf& operator<<(uint64 i) { Add(i); return *this; }
        Printf& operator<<(float f) { Add(f); return *this; }
        Printf& operator<<(double f) { Add(f); return *this; }

        uint32 getBufferSize() const { return mBuffer.GetSize(); }
        const char* getBuffer() const { return mBuffer.GetData(); }

    private:
        static const uint32 STACK_BUFFER = 64;
        TArray<char> mBuffer;
    };

}

#endif //BERSERK_PRINTF_HPP