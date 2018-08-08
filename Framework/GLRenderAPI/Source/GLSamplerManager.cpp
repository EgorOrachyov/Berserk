//
// Created by Egor Orachyov on 08.08.2018.
//

#include "Managers/GLSamplerManager.h"

namespace Berserk
{

    GLSamplerManager::GLSamplerManager()
    {
        mSamplerList.init();
    }

    UINT32 GLSamplerManager::getMemoryUsage() const
    {
        UINT32 memUsage = 0;

        if (mSamplerList.getSize())
        {
            memUsage += mSamplerList.getSize() * mSamplerList.getFirst().getMemoryUsage();
        }

        return memUsage;
    }

    GLSampler* GLSamplerManager::createSampler()
    {
        GLSampler sampler;
        sampler.addReference();
        mSamplerList.add(sampler);
        return &mSamplerList.getLast();
    }

    GLSampler* GLSamplerManager::getSampler(UINT32 id) const
    {
        return &mSamplerList.get(id);
    }

    bool GLSamplerManager::deleteSampler(GLSampler* toDelete)
    {
        GLSampler *sampler = nullptr;

        mSamplerList.iterate(true);
        while (mSamplerList.iterate())
        {
            if (mSamplerList.getCurrent().getHandle() == toDelete->getHandle())
            {
                sampler = toDelete;
                break;
            }
        }

        if (sampler == nullptr)
        {
            return false;
        }

        sampler->release();
        if (sampler->getReferences())
        {
            mSamplerList.remove(*sampler);
        }

        return true;
    }

} // namespace Berserk