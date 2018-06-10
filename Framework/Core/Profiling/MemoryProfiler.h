//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_MEMORYMANAGER_H
#define BERSERKENGINE_MEMORYMANAGER_H

#include "../Essential/Assert.h"
#include "../Essential/Types.h"
#include "../Loggin/LogManager.h"

namespace Berserk
{
    /**
     *  @brief Engine memory profiling
     *
     *  Profiles all the used memory in the application and covers
     *  all the systems and sub systems
     */
    class MemoryProfiler
    {
    private:

        typedef struct SystemInfo
        {
        public:

            SystemInfo()
            {
                mAllocatedMemory = 0;
                mLimitation = 0;
                mMemoryCalls = 0;
            }

            void AddAllocatedMemory(int32 bytes)
            {
                ASSERT(bytes > 0, "Bytes should be more than 0");

                mAllocatedMemory += bytes + (MEMORY_ALIGNMENT - (bytes % MEMORY_ALIGNMENT));
                mMemoryCalls += 1;
            }

            void ReduceAllocatedMemory(int32 bytes)
            {
                ASSERT(bytes < 0, "Bytes should be less than 0");

                mAllocatedMemory -= bytes - (MEMORY_ALIGNMENT + (bytes % MEMORY_ALIGNMENT));
                mMemoryCalls += 1;
            }

            void SetLimitation(int32 limitation)
            {
                ASSERT(limitation >= 0, "Limitation cannot be less than 0");
                mLimitation = limitation;
            }

            void SetSystemName(char * name)
            {
                strcpy(mSysName, name);
            }

            int32 GetMemoryCalls()
            {
                return mMemoryCalls;
            }

            int32 GetAllocatedMemory()
            {
                return mAllocatedMemory;
            }

            int32 GetLimitation()
            {
                return mLimitation;
            }

            void PushInfo(char * destination) {
                char allocMem[BUFFER_SIZE_16];
                char availMem[BUFFER_SIZE_16];
                char limitation[BUFFER_SIZE_16];

                PushInfoAboutBytes(allocMem, mAllocatedMemory);

                if (mLimitation == DEBUG_NO_MEM_LIMITATION) {
                    sprintf(limitation, "No Limit");
                    sprintf(availMem, "No Limit");
                } else {
                    PushInfoAboutBytes(limitation, mLimitation);
                    PushInfoAboutBytes(availMem, mLimitation - mAllocatedMemory);
                }

                sprintf(destination, "%32s: %16s | %16s | %16s | %16i |", mSysName, allocMem, availMem, limitation, mMemoryCalls);
            }

        private:

            int32 mAllocatedMemory;
            int32 mLimitation;
            int32 mMemoryCalls;
            char  mSysName[BUFFER_SIZE_32];

        } SystemInfo;

    public:

        MemoryProfiler();

        /**
         * Add allocated bytes to chosen system
         * (bytes will be aligned by MEMORY_ALIGNMENT)
         *
         * @param system Target system
         * @param bytes Bytes to add (bytes > 0)
         */
        void AddAllocatedMemory(int16 system, int32 bytes);

        /**
         * Reduce allocated bytes to chosen system
         * (bytes will be aligned by MEMORY_ALIGNMENT)
         *
         * @param system Target system
         * @param bytes Bytes to reduce (bytes < 0)
         */
        void ReduceAllocatedMemory(int16 system, int32 bytes);

        /**
         * Limitation for memory using by this custem
         *
         * @param system Target system
         * @param limitation Desired limitation (limitation >= 0)
         *        limitation = 0 <=> DEBUG_NO_MEM_LIMITATION
         */
        void SetLimitation(int16 system, int32 limitation);

        /**
         * Pushes info about used memory into global engine log
         */
        void PushInfoIntoLog();

        /**
         * Pushes notification about itself into global info log
         */
        static void PushNoteToLog();

        /**
         * Writes bytes value in string in 'human-visible' form
         * (in KiB, MiB, GiB, bytes ...)
         *
         * @param destination Array of chars to put info
         * @param value Num of bytes which will be converted
         *        in C string interpretation
         */
        static void PushInfoAboutBytes(char* destination, int32 value);

    private:

        SystemInfo mSystems[DEBUG_NUM_OF_SYSTEMS];

    };

    /**
     * Global Singleton Memory Manager
     */
    extern MemoryProfiler globalMemoryProfiler;

} // namespace Berserk

#endif //BERSERKENGINE_MEMORYMANAGER_H
