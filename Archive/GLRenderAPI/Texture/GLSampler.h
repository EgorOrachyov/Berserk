//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLSAMPLERBUFFER_H
#define BERSERKENGINE_GLSAMPLERBUFFER_H

#include "GLTextureWrapping.h"
#include "GLTextureFiltering.h"

#include "Misc/Types.h"

namespace Berserk
{

    class GLSampler
    {
    public:

        GLSampler();
        ~GLSampler();

        UINT32 getMemoryUsage() const;

        void   addReference();
        void   release();
        UINT32 getReferences() const;

        void init();
        void init(GLWrapping wrapping, GLFiltering filtering);
        void destroy();

        void use(UINT32 targetTextureSlot) const;

        void setWrapping(GLWrapping s, GLWrapping t);
        void setFiltering(GLFiltering min, GLFiltering mag);

        UINT32 getHandle() const;

        const bool operator == (const GLSampler& sampler) const;

    private:

        UINT32 mReferenceCount;
        UINT32 mHandle;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSAMPLERBUFFER_H