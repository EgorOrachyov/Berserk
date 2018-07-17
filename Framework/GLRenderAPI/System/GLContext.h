//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLCONTEXT_H
#define BERSERKENGINE_GLCONTEXT_H

#include "Essential/Types.h"

namespace Berserk
{

    /**
     *
     */
    class GLContext
    {
    public:

        GLContext();
        ~GLContext();

        void initWindowContext();

        void initRenderingContext();

        void destroy();

        bool isInitialized() const;

        void printContextInfo() const;

    protected:

        INT8 mIsInitialized;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLCONTEXT_H
