/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPAIR_H
#define BERSERK_TPAIR_H

#include <Typedefs.h>
#include <IO/Archive.h>
#include <Platform/Memory.h>

namespace Berserk {

    template <typename A, typename B>
    class TPair final {
    public:
        TPair() {
            new (&first()) A();
            new (&second()) B();
        }
        ~TPair() {
            first().~A();
            second().~B();
        }
        TPair(const A& a, const B& b) {
            new (&first()) A(a);
            new (&second()) B(b);
        }
        TPair(const TPair& other) {
            new (&first()) A(other.first());
            new (&second()) B(other.second());
        }
        TPair(TPair&& other) noexcept {
            new (&first()) A(std::move(other.first()));
            new (&second()) B(std::move(other.second()));
        }

        TPair& operator=(const TPair& other) {
            new (&first()) A(other.first());
            new (&second()) B(other.second());
        }
        TPair& operator=(TPair&& other) noexcept {
            new (&first()) A(std::move(other.first()));
            new (&second()) B(std::move(other.second()));
        }

        A& first() { return *((A*)(mBuffer + 0)); }
        const A& first() const { return *((A*)(mBuffer + 0)); }

        B& second() { return *((B*)(mBuffer + sizeof(A))); }
        const B& second() const { return *((B*)(mBuffer + sizeof(A))); }

        friend Archive& operator<<(Archive& archive, const TPair& pair) {
            if (archive.getType() == EArchiveType::Binary) {
                archive << pair.first();
                archive << pair.second();
            }

            return archive;
        }

        friend Archive& operator>>(Archive& archive, TPair& pair) {
            if (archive.getType() == EArchiveType::Binary) {
                archive >> pair.first();
                archive >> pair.second();
            }

            return archive;
        }

    private:
        uint8 mBuffer[sizeof(A) + sizeof(B)] = {};
    };

}

#endif //BERSERK_TPAIR_H