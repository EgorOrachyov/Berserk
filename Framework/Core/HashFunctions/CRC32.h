//
// Created by Egor Orachyov on 01.07.2018.
//

#ifndef BERSERKENGINE_CRC32_H
#define BERSERKENGINE_CRC32_H

#include "../Essential/Types.h"

namespace Berserk
{

    /**
     * CRC-32 hash function for float32 value
     *
     * @param value To be hashed
     * @return uint32 hash
     */
    uint32 CRC32(float32 value);

    /**
     * CRC-32 hash function for int32 value
     *
     * @param value To be hashed
     * @return uint32 hash
     */
    uint32 CRC32(int32 value);

    /**
     * CRC-32 hash function for chars array
     *
     * @param buffer Pointer to buffer
     * @param len Number of chars
     * @return uint32 hash
     */
    uint32 CRC32(const CHAR* buffer, int32 len);

} // namespace Berserk

#endif //BERSERKENGINE_CRC32_H