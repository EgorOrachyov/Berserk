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
    UINT32 hashCRC32(FLOAT32 value);

    /**
     * CRC-32 hash function for int32 value
     *
     * @param value To be hashed
     * @return uint32 hash
     */
    UINT32 hashCRC32(INT32 value);

    /**
     * CRC-32 hash function for float64 value
     *
     * @param value To be hashed
     * @return uint32 hash
     */
    UINT32 hashCRC32(FLOAT64 value);

    /**
     * CRC-32 hash function for int64 value
     *
     * @param value To be hashed
     * @return uint32 hash
     */
    UINT32 hashCRC32(INT64 value);

    /**
     * CRC-32 hash function for chars array
     *
     * @param buffer Pointer to buffer
     * @param len Number of chars
     * @return uint32 hash
     */
    UINT32 hashCRC32(const CHAR *buffer, INT32 len);

} // namespace Berserk

#endif //BERSERKENGINE_CRC32_H