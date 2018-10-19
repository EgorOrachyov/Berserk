//
// Created by Egor Orachyov on 10.07.2018.
//

#ifndef BERSERKENGINE_CSTRINGBUFFER_H
#define BERSERKENGINE_CSTRINGBUFFER_H

#include "StringFlags.h"
#include "Misc/Types.h"
#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * Storage for chars buffers for c strings of different sizes.
     * @warning Use only for strings
     */
    class CStringBuffer
    {
    public:

        /**
         * Init all the internal pool allocators
         */
        CStringBuffer();

        /**
         * Free all used data of allocators' buffers
         */
        ~CStringBuffer();

        /**
         * Init buffer
         * @note this function are equal constructor doings
         */
        void init();

        /**
         * Destroy buffer
         * @note this function are equal destructor doings
         */
        void destroy();

        /**
         * Total count of strings in buffer
         * @return
         */
        UINT32 getStringsCount() const;

        /**
         * Is buffer internal pools are initialized
         * @return
         */
        bool isInitialized() const;

    private:

        /**
         * Allows c string return used block
         * @param size Block size
         * @param block Pointer to block
         */
        void returnBlock(UINT32 size, CHAR* block);

        /**
         * Allows c string get new buffer block
         * @param size Block size
         * @return pointer to buffer
         */
        CHAR* getBlock(UINT32 size);

        /**
         * Get global string buffer
         * @return Link to buffer
         */
        static CStringBuffer& get();

    private:

        friend class CString;

        PoolAllocator mPool[SBC_CSTRING_BUFFERS_COUNT];     // Pools with strings of concrete size
        UINT32 mStringsCount;                               // Count of allocated blocks
        INT8   mIsInitialized;                              // Initialization status

    };

} // namespace Berserk

#endif //BERSERKENGINE_CSTRINGBUFFER_H