//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLWINDOW_H
#define BERSERKENGINE_GLWINDOW_H

#include "Essential/GLInclude.h"

#include "Essential/Types.h"

namespace Berserk
{

    /**
     *
     */
    class GLWindow
    {
    public:

        GLWindow();
        ~GLWindow();

        void create();
        void create(UINT32 width, UINT32 height, const CHAR* name);

        void destroy();

        void makeCurrent() const;

        GLFWwindow* getHandle() const;

        UINT32 getWidth() const;

        UINT32 getHeight() const;

        bool isCreated() const;

    protected:

        GLFWwindow* mHandle;
        UINT32 mWidth;
        UINT32 mHeight;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLWINDOW_H
