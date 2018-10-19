//
// Created by Egor Orachyov on 10.07.2018.
//

#ifndef BERSERKENGINE_WSTRINGBUFFER_H
#define BERSERKENGINE_WSTRINGBUFFER_H

#include "StringFlags.h"
#include "Misc/Types.h"
#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * Storage for chars buffers for w strings of different sizes.
     * @warning Use only for strings
     */
    class WStringBuffer
    {
    public:

        /**
         * Init all the internal pool allocators
         */
        WStringBuffer();

        /**
         * Free all used data of allocators' buffers
         */
        ~WStringBuffer();

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
         * Allows w string return used block
         * @param size Block size
         * @param block Pointer to block
         */
        void returnBlock(UINT32 size, WCHAR* block);

        /**
         * Allows w string get new buffer block
         * @param size Block size
         * @return pointer to buffer
         */
        WCHAR* getBlock(UINT32 size);

        /**
         * Get global string buffer
         * @return Link to buffer
         */
        static WStringBuffer& get();

    private:

        friend class WString;

        PoolAllocator mPool[SBC_WSTRING_BUFFERS_COUNT];     // Pools with strings of concrete size
        UINT32 mStringsCount;                               // Count of allocated blocks
        INT8   mIsInitialized;                              // Initialization status

    };

} // namespace Berserk

#endif //BERSERKENGINE_WSTRINGBUFFER_H
