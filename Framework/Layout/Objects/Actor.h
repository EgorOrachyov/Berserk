//
// Created by Egor Orachyov on 25.07.2018.
//

#ifndef BERSERKENGINE_ACTOR_H
#define BERSERKENGINE_ACTOR_H

#include <Math/Matrix4x4f.h>
#include "Objects/Object.h"

namespace Berserk
{

    class Actor : public Object
    {
    public:

        Actor(const CStaticString& name);
        Actor(const CStaticString& name, UINT32 lifeTime);
        virtual ~Actor();

        void update(const Matrix4x4f& rootTransformation);

    private:

        INT8 mIsActive;
        UINT32 mLifeTime;

    };

} // namespace Berserk

#endif //BERSERKENGINE_ACTOR_H