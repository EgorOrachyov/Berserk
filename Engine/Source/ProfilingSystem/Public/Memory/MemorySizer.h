//
// Created by Egor Orachyov on 31.03.2019.
//

#ifndef BERSERK_MEMORYSIZER_H
#define BERSERK_MEMORYSIZER_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Containers/HashMap.h"

namespace Berserk
{

    /**
     * Allows to profile per object memory usage on CPU and GPU side.
     * Requires only the object has const char pointer to its name to navigate
     * through profiled objects.
     *
     * Usage:
     *   root = addObject(name, cpu_memory_usage, gpu_memory_usage);
     *   for (each child)
     *   {
     *      addChild(root, child.name, child.cpu, child.gpu);
     *   }
     *
     * For each child component of the object there is also
     * possible recursive profiling.
     */
    class ENGINE_API MemorySizer
    {
    public:

        struct ProfilingNode
        {
        public:

            ProfilingNode(const char* name, uint32 cpuMem, uint32 gpuMem)
                    : mObjectName(name),
                      mCpuMem(cpuMem),
                      mGpuMem(gpuMem),
                      mNextNode(nullptr),
                      mSubNode(nullptr),
                      mParent(nullptr)
            {
                ASSERT(name, "Null pointer name");
            }

            uint32 getCpuMem() { return mCpuMem; }
            uint32 getGpuMem() { return mGpuMem; }

            ProfilingNode* getNextNode() { return mNextNode; }
            ProfilingNode* getSubNode()  { return mSubNode; }

        private:

            friend class MemorySizer;

            uint32 mCpuMem;
            uint32 mGpuMem;

            ProfilingNode* mParent;     // Parent node (stays higher in the hierarchy)
            ProfilingNode* mNextNode;   // Next in current hierarchy
            ProfilingNode* mSubNode;    // First in new child -> hierarchy of this node
            const char*    mObjectName; // Object name (copy for pretty printing)
        };

    public:

        /** Initialize internal storage */
        MemorySizer();

        ~MemorySizer() = default;

        /**
         * Add profiling object in sizer storage
         * @param name   Pointer to the system name
         * @param cpuMem Its memory acquirement on CPU side
         * @param gpuMem Its memory acquirement on GPU side (optional)
         */
        ProfilingNode* addObject(const char *name, uint32 cpuMem, uint32 gpuMem = 0);

        /**
         * Add another child object for chosen node
         *
         * + node
         * +-+ child object
         */
        ProfilingNode* addChild(ProfilingNode *node, const char *name, uint32 cpuMem, uint32 gpuMem = 0);

        /**
         * Add next object (neighbor) for chosen node
         *
         * + node
         * + next object
         */
        ProfilingNode* addNext(ProfilingNode *node, const char *name, uint32 cpuMem, uint32 gpuMem = 0);

        /** @return Pointer ot internal data */
        HashMap<const char*, ProfilingNode>* getProfilingInfo();

        /** Out all info in default console */
        void print(uint32 alignment = 32);

    private:

        PoolAllocator mPoolAllocator;
        HashMap<const char*, ProfilingNode> mProfilingInfo;

    };

} // namespace Berserk

#endif //BERSERK_MEMORYSIZER_H