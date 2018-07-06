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
        void create(uint32 width, uint32 height, const CHAR* name);

        void destroy();

        void makeCurrent() const;

        GLFWwindow* getHandle() const;
        uint32 getWidth() const;
        uint32 getHeight() const;

        bool isCreated() const;

    protected:

        GLFWwindow* mHandle;
        uint32 mWidth;
        uint32 mHeight;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLWINDOW_H
