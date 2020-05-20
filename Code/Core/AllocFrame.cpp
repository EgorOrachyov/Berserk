/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <AllocFrame.h>
#include <BuildOptions.h>

namespace Berserk {

    inline uint64 alignedSize(uint64 size, uint64 alignment) {
        return size + (size % alignment? alignment - (size % alignment): 0);
    }

    AllocFrame::AllocFrame(Berserk::uint64 frameSize, Berserk::IAlloc &alloc) {
        mAlloc = &alloc;
        mFrameSize = frameSize;
    }

    AllocFrame::~AllocFrame() {
        for (auto& frame: mFrames) {
            mAlloc->free(frame.begin);
        }
        mFrames.clearNoDestructorCall();
    }

    void *AllocFrame::allocate(uint64 size) {
        size = alignedSize(size, BERSERK_PLATFORM_ALIGN);
        BERSERK_COND_ERROR_RET_VALUE(nullptr, size <= mFrameSize, "Allocation size is out of frame size")

        Frame* frame = getFrame(size);
        BERSERK_COND_ERROR_RET_VALUE(nullptr, frame != nullptr, "Failed to allocate memory in frame")

        uint8* mem = (uint8*)frame->begin + frame->allocated;
        frame->allocated += size;
        mAllocated += size;

        return mem;
    }

    void AllocFrame::free(void *memory) {
        // do nothing, since only reset can mark all frames as free
    }

    void AllocFrame::reset() {
        for (auto& frame: mFrames) {
            frame.allocated = 0;
        }

        mAllocated = 0;
    }

    AllocFrame::Frame * AllocFrame::getFrame(uint64 freeSpace) {
        if (mFrames.size() == 0) return allocAnotherFrame();

        for (auto& frame: mFrames) {
            if (frame.allocated + freeSpace <= mFrameSize)
                return &frame;
        }

        return allocAnotherFrame();
    }

    AllocFrame::Frame* AllocFrame::allocAnotherFrame() {
        if (mFrames.size() == MAX_ALLOC_FRAMES)
            return nullptr;

        auto& frame = mFrames.emplace();
        frame.allocated = 0;
        frame.begin = mAlloc->allocate(mFrameSize);

        return &frame;
    }



}