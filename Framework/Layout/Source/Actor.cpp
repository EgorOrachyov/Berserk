//
// Created by Egor Orachyov on 25.07.2018.
//

#include "Objects/Actor.h"

namespace Berserk
{

    Actor::Actor(const CStaticString& name) : Object(name)
    {
        mIsActive = true;
        mLifeTime = 0;
    }

    Actor::Actor(const CStaticString& name, UINT32 lifeTime) : Object(name)
    {
        mIsActive = true;
        mLifeTime = lifeTime;
    }

    Actor::~Actor()
    {

    }

    void Actor::update(const Matrix4x4f& rootTransformation)
    {

    }

} // namespace Berserk