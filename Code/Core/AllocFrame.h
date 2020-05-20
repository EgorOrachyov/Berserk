/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ALLOCFRAME_H
#define BERSERK_ALLOCFRAME_H

#include <Containers/TArrayStatic.h>

namespace Berserk {

    /**
     * @brief Frame allocator
     *
     * Allocates memory in the continuous linear memory region.
     * Allows to free all the allocated memory at once without fragmentation, i.e. does not
     * support partial free by allocated memory blocks.
     *
     * Allocates additional memory regions on demand if current region is full.
     *
     * @note Allocation time O(1)
     * @note Free time O(1)
     */
    class AllocFrame final: public IAlloc  {
    public:
        /** Default frame size in bytes (1 MiB) */
        static const uint64 DEFAULT_FRAME_SIZE = 1024 * 1024;
        static const uint64 MAX_ALLOC_FRAMES = 4;

        AllocFrame(uint64 frameSize = DEFAULT_FRAME_SIZE, IAlloc& alloc = IAlloc::getSingleton());
        ~AllocFrame() override;

        void *allocate(uint64 size) override;
        void free(void *memory) override;
        void reset();

        uint64 getAllocated() const { return mAllocated; }
        uint64 getFrameSize() const { return mFrameSize; };
        uint64 getFramesCount() const { return mFrames.size(); };
    private:

        struct Frame {
            uint64 allocated;
            void* begin;
        };

        AllocFrame::Frame *getFrame(uint64 freeSpace);
        Frame* allocAnotherFrame();

        IAlloc* mAlloc;
        uint64 mFrameSize;
        uint64 mAllocated = 0;
        TArrayStatic<Frame,MAX_ALLOC_FRAMES> mFrames;
    };

}

#endif //BERSERK_ALLOCFRAME_H