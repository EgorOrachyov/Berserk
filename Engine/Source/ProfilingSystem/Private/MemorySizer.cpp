//
// Created by Egor Orachyov on 31.03.2019.
//

#include "MemorySizer.h"
#include "Profiling/ProfilingUtility.h"
#include <Strings/String.h>

namespace Berserk
{

    MemorySizer::MemorySizer()
            : mPoolAllocator(HashMap<const char*, ProfilingNode>::getNodeSize(), PoolAllocator::INITIAL_CHUNK_COUNT),
              mProfilingInfo(Strings<char,'\0'>::Hashing, &mPoolAllocator)
    {

    }

    MemorySizer::ProfilingNode * MemorySizer::addObject(const char *name, uint32 cpuMem, uint32 gpuMem)
    {
        mProfilingInfo.add(name, ProfilingNode(name, cpuMem, gpuMem));
        return mProfilingInfo[name];
    }

    MemorySizer::ProfilingNode * MemorySizer::addChild(ProfilingNode *node, const char *name, uint32 cpuMem,
                                                       uint32 gpuMem)
    {
        if (node->mSubNode == nullptr)
        {
            auto sub = addObject(name, cpuMem, gpuMem);

            node->mSubNode = sub;
            sub->mParent = node;

            return sub;
        }
        else
        {
            auto sub = addObject(name, cpuMem, gpuMem);

            node->mSubNode->mParent = sub;
            sub->mNextNode = node->mSubNode;
            node->mSubNode = sub;
            sub->mParent = node;

            return sub;
        }
    }

    MemorySizer::ProfilingNode * MemorySizer::addNext(ProfilingNode *node, const char *name, uint32 cpuMem,
                                                      uint32 gpuMem)
    {
        if (node->mNextNode == nullptr)
        {
            auto next = addObject(name, cpuMem, gpuMem);

            node->mNextNode = next;
            next->mParent = node;

            return next;
        }
        else
        {
            auto next = addObject(name, cpuMem, gpuMem);

            node->mNextNode->mParent = next;
            next->mNextNode = node->mNextNode;
            node->mNextNode = next;
            next->mParent = node;

            return next;
        }
    }

    HashMap<const char *, MemorySizer::ProfilingNode> * MemorySizer::getProfilingInfo()
    {
        return &mProfilingInfo;
    }

    void MemorySizer::print(uint32 alignment)
    {
        for (auto current = getProfilingInfo()->iterate(); current; current = getProfilingInfo()->next())
        {
            char cpu[Buffers::SIZE_16];
            char gpu[Buffers::SIZE_16];

            auto root = current->value();
            uint32 rootOffset = 0;

            // printf("Node: name=%s sub=%p next=%p parent=%p \n", root.mObjectName, root.mSubNode, root.mNextNode, root.mParent);

            if (root.mSubNode != nullptr)
            {
                printf("%s %s %*s: CPU %12s | GPU %12s \n", "+",
                       current->key(),
                       alignment - Strings<char, '\0'>::strlen(current->key()), "",
                       ProfilingUtility::print(current->value().getCpuMem(), cpu),
                       ProfilingUtility::print(current->value().getGpuMem(), gpu));

                uint32 offset = rootOffset + 10;

                for (auto child = root.mSubNode; child != nullptr; child = child->mNextNode)
                {
                    printf("%s %s %*s: CPU %12s | GPU %12s \n", "+-+",
                           child->mObjectName,
                           alignment - 2 - Strings<char, '\0'>::strlen(child->mObjectName), "",
                           ProfilingUtility::print(child->getCpuMem(), cpu),
                           ProfilingUtility::print(child->getGpuMem(), gpu));
                }
            }
        }
    }

} // namespace Berserk