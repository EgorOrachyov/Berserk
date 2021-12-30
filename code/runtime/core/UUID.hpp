/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_UUID_HPP
#define BERSERK_UUID_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>

#include <ostream>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class UUID
 * @brief 16-bytes universally unique identifier
 */
class UUID final {
public:
    BRK_API UUID() = default;
    BRK_API explicit UUID(uint32 a, uint32 b, uint32 c, uint32 d);
    BRK_API explicit UUID(const String &uuid);

    BRK_API const uint32 *Words() const { return mWords; }
    BRK_API String ToString() const;
    BRK_API uint32 Hash() const;

    static BRK_API UUID Empty();
    static BRK_API UUID Generate();

private:
    uint32 mWords[4] = {0, 0, 0, 0};
};

/**
 * @}
 */

BRK_NS_END

namespace std {

    template<>
    struct hash<BRK_NS::UUID> {
    public:
        size_t operator()(const BRK_NS::UUID &uuid) const {
            return uuid.Hash();
        }
    };

}// namespace std

inline std::ostream &operator<<(std::ostream &stream, const BRK_NS::UUID &name) {
    stream << name.ToString();
    return stream;
}

#endif//BERSERK_UUID_HPP
