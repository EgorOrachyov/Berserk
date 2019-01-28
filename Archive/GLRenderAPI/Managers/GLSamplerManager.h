//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_GLSAMPLERMANAGER_H
#define BERSERKENGINE_GLSAMPLERMANAGER_H

#include "Texture/GLSampler.h"
#include "Containers/LinkedList.h"

namespace Berserk
{

    class GLSamplerManager
    {
    public:

        GLSamplerManager();
        ~GLSamplerManager() = default;

        UINT32 getMemoryUsage() const;

        GLSampler* createSampler();
        GLSampler* getSampler(UINT32 id) const;

        bool deleteSampler(GLSampler* toDelete);

    private:

        LinkedList<GLSampler> mSamplerList;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSAMPLERMANAGER_H