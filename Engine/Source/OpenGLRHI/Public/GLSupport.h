//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLSUPPORT_H
#define BERSERK_GLSUPPORT_H

namespace Berserk
{

    class GLSupport : public RHISupport
    {
    public:

        ~GLSupport() override = default;

        Mat4x4f projectionMatrix(float32 fovy, float32 aspect, float32 near, float32 far) override
        {
            return Mat4x4f::perspective_OpenGL(fovy, aspect, near, far);
        }

    };

} // namespace Berserk

#endif //BERSERK_GLSUPPORT_H
