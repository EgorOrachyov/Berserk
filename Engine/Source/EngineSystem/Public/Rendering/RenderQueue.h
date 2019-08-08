//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERQUEUE_H
#define BERSERK_RENDERQUEUE_H

#include <Rendering/RenderElement.h>

namespace Berserk
{

    /** Node, contains actual render data pointer and sortable data */
    struct RENDER_API RenderQueueNode : public Allocatable
    {
    public:

        RenderQueueNode(const RenderElement& info,
                        float32 distanceFromView,
                        float32 materialID)
            : info(info),
              distanceFromView(distanceFromView),
              materialID(materialID)
        {

        }

        /** Pointer to valid data of the object [use pointer to be lightweight] */
        const RenderElement& info;

        /** Non-negative distance from camera [defines explicitly] */
        float32 distanceFromView;

        /** All the materials, loaded throw resource manager has unique id */
        uint32 materialID;

    };

    /** Sort options for queue*/
    enum ERenderQueueSortOptions
    {
        /** Objects will be sorted from lower to higher depth */
        RQSO_Distance
    };

    /**
     * Rendering queue is supposed to sort render object for single point of the view in the scene
     * and group this object in order to increase performance. Materials, render object
     * options and distance from the camera and sort options defines sorted objects order.
     */
    class RENDER_API RenderQueue
    {
    public:

        /**
         * Initialize queue with desired sort options and specified allocator
         * @param options Sorting preferred order
         * @param nodesCount
         * @param allocator To allocate internal queue
         */
        explicit RenderQueue(ERenderQueueSortOptions options = RQSO_Distance,
                uint32 nodesCount = DEFAULT_SORT_NODES_COUNT,
                IAllocator& allocator = Allocator::get());

        ~RenderQueue() = default;

        /**
         * Add render object to the queue to be sorted
         * @param renderable Pointer to data of the object
         * @param viewDistance Computed distance from view point
         */
        void add(const RenderElement& element, float32 viewDistance);

        /** Sorts all the elements in the queue */
        void sort();

        /** Clears queue before next submit session */
        void clear();

        /** @return Sort options of that queue */
        ERenderQueueSortOptions getSortOptions() const { return mSortOptions; }

        /** @return Actual nodes data */
        const TArray<RenderQueueNode> &getNodes() const { return mQueue; }

    private:

        /** Sort operation for queue */
        typedef TArray<RenderQueueNode>::Predicate SortOperation;

        /** Sort material first then distance in '<' order */
        static bool sortDistance(const RenderQueueNode& a, const RenderQueueNode& b)
        {
            if (a.materialID == b.materialID)
                return a.distanceFromView < b.distanceFromView;
            else
                return a.materialID < b.materialID;
        }

    private:

        /** To preallocate memory for queue objects */
        static const uint32 DEFAULT_SORT_NODES_COUNT = 100;

        /** Sort options of that queue */
        ERenderQueueSortOptions mSortOptions;

        /** Actual nodes data */
        TArray<RenderQueueNode> mQueue;

    };

} // namespace Berserk

#endif //BERSERK_RENDERQUEUE_H