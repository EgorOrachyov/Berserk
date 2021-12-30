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

#include <core/Crc32.hpp>
#include <core/UUID.hpp>

#include <chrono>
#include <random>

BRK_NS_BEGIN

static const char MAP_HEX_TO_LITERAL[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static const uint8 MAP_LITERAL_TO_HEX[256] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        // 0 through 9 translate to 0  though 9
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        // A through F translate to 10 though 15

        0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        // a through f translate to 10 though 15
        0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

UUID::UUID(uint32 a, uint32 b, uint32 c, uint32 d) {
    mWords[0] = a;
    mWords[1] = b;
    mWords[2] = c;
    mWords[3] = d;
}

UUID::UUID(const String &uuid) : UUID(0, 0, 0, 0) {
    // Parse string:
    // - mask: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    // - exmp: 123e4567-e89b-12d3-a456-426614174000

    if (uuid.size() < 36)
        return;

    auto *bytes = reinterpret_cast<uint8 *>(mWords);

    uint32 id = 0;
    uint32 byte = 0;

    auto parseBlock = [&](uint32 count) {
        for (uint32 i = 0; i < count / 2; ++i) {
            auto charValue1 = uuid[id++];
            auto charValue2 = uuid[id++];
            auto mapping1 = MAP_LITERAL_TO_HEX[charValue1];
            auto mapping2 = MAP_LITERAL_TO_HEX[charValue2];
            bytes[byte++] = (mapping1 << 4u) | mapping2;
        }
    };

    parseBlock(8);
    id += 1;

    parseBlock(4);
    id += 1;

    parseBlock(4);
    id += 1;

    parseBlock(4);
    id += 1;

    parseBlock(12);
}

String UUID::ToString() const {
    char buffer[36];
    const auto *bytes = reinterpret_cast<const uint8 *>(mWords);

    uint32 id = 0;
    uint32 byte = 0;

    auto writeBytes = [&](uint32 count) {
        for (uint32 i = 0; i < count / 2; i++) {
            auto value = bytes[byte++];
            auto part1 = 0xFu & (value >> 4u);
            auto part2 = 0xFu & value;
            buffer[id++] = MAP_HEX_TO_LITERAL[part1];
            buffer[id++] = MAP_HEX_TO_LITERAL[part2];
        }
    };

    writeBytes(8);
    buffer[id++] = '-';

    writeBytes(4);
    buffer[id++] = '-';

    writeBytes(4);
    buffer[id++] = '-';

    writeBytes(4);
    buffer[id++] = '-';

    writeBytes(12);

    return String(buffer, 36);
}

uint32 UUID::Hash() const {
    Crc32Builder builder;
    builder.Hash(mWords, sizeof(uint32) * 4);
    return builder.GetHash();
}

UUID UUID::Empty() {
    return UUID(0, 0, 0, 0);
}

UUID UUID::Generate() {
    std::default_random_engine engine(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<uint32> distribution;
    return UUID(distribution(engine), distribution(engine), distribution(engine), distribution(engine));
}

BRK_NS_END