//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_RENDERTARGETMANAGER_H
#define BERSERK_RENDERTARGETMANAGER_H

#include <RHI/RHIDriver.h>
#include <Containers/TLinkedList.h>
#include <Rendering/RenderTarget.h>
#include <Math/Size.h>

namespace Berserk
{

    /**
     * Allocates and stores all the render targets needed render system
     * to render single scene view.
     *
     * @note Handles internally all the allocations
     * @note Supports HDR frame buffers creating
     * @note Allows to resize frame buffers
     * @note Maps frame buffers via its string look-up name
     */
    class RENDER_API RenderTargetManager : public Allocatable
    {
    public:

        /**
         * Creates render target manager for specified rhi driver
         * @param driver RHI driver to create actual GPU targets
         * @param limit Default limitation on size of render targets
         * @param allocator Memory manager for internal usage
         */
        RenderTargetManager(RHIDriver& driver, Size limit = DEFAULT_LIMIT, IAllocator& allocator = Allocator::get());

        ~RenderTargetManager();

        /**
         * Creates new render with specified params
         *
         * @note If render target with specified name already exist - returns that target
         * @note Handles allocation internally, no external free required
         *
         * @param name Target descriptive name
         * @param view Size of the buffer
         * @param format Pixel format
         * @param isHDR If this target supports hdr rendering
         * @return Target or null if there is some kind of error
         */
        RenderTarget* create(const char* name, Size size, EStorageFormat format, bool isHDR);

        /**
         * Finds render target with specified name
         * @param name To find
         * @return Target or null if not found
         */
        RenderTarget* find(const char* name);

        /** @return Max buffer size, which could be created by manager */
        const Size& getLimit() const { return mLimit; }

    public:

        /** Default limitation for created render targets (w=10000,h=10000) */
        static const Size DEFAULT_LIMIT;

    private:

        typedef TLinkedList<RenderTarget> TargetsList;

        Size mLimit;
        RHIDriver& mDriver;
        PoolAllocator mPool;
        TargetsList mTargetsList;

    };

} // namespace Berserk

#endif //BERSERK_RENDERTARGETMANAGER_H