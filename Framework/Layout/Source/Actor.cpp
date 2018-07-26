//
// Created by Egor Orachyov on 25.07.2018.
//

#include "Objects/Actor.h"
#include "Math/UtilityMatrices.h"

namespace Berserk
{

    Actor::Actor(const CStaticString &name, FLOAT32 lifeTime) : Object(name)
    {
        mIsActive = true;
        mIsEditable = true;
        mIsPaused = false;
        mIsVisible = true;
        mIsAttachable = true;

        mLifeTime = lifeTime;

        mRoot = NULL;
        mChildren.init(2);    // ensure capacity for 4 elements

        mTransformation = newMatrix(1);
    }

    Actor::~Actor()
    {

    }

    void Actor::onStart()
    {

    }

    void Actor::onUpdate(FLOAT64 elapsedTime)
    {

    }

    void Actor::onReset()
    {

    }

    void Actor::onEnd()
    {

    }

    void Actor::attachActor(Actor *actor)
    {
        if (actor == NULL)
        {
            WARNING("An attempt to attach NULL Actor");
            return;
        }
        if (mIsAttachable && mIsEditable)
        {
            mChildren.add(actor);
            actor->mRoot = this;
        }
    }

    const Actor* Actor::getRoot() const
    {
        return mRoot;
    }

    const ArrayList<Actor*> &Actor::getChildren() const
    {
        return mChildren;
    }

    const Matrix4x4f& Actor::getTransformation() const
    {
        return mTransformation;
    }

    const Matrix4x4f& Actor::getAbsoluteTransformation() const
    {
        Matrix4x4f result = mTransformation;

        const Actor* tmp = this;
        while (tmp->getRoot())
        {
            tmp = tmp->getRoot();
            result = tmp->getTransformation() * result;
        }

        return result;
    }

    void Actor::setTransformation(const Matrix4x4f &transformation)
    {
        if (mIsEditable)
        {
            mTransformation = transformation;
        }
    }

    void Actor::addMovement(const Vector3f &translation)
    {
        if (mIsEditable)
        {
            mTransformation = translate(translation) * mTransformation;
        }
    }

    void Actor::addRotation(const Vector3f &axis, FLOAT32 angle)
    {
        if (mIsEditable)
        {
            mTransformation = rotate(axis, angle) * mTransformation;
        }
    }

    void Actor::addScale(FLOAT32 factor)
    {
        if (mIsEditable)
        {
            mTransformation = scale(factor, factor, factor) * mTransformation;
        }
    }

    void Actor::setActive(bool setIn)
    {
        mIsActive = setIn;
    }

    void Actor::setVisible(bool setIn)
    {
        mIsVisible = setIn;
    }

    void Actor::setPaused(bool setIn)
    {
        mIsPaused = setIn;
    }

    void Actor::setEditable(bool setIn)
    {
        mIsEditable = setIn;
    }

    void Actor::setAttachable(bool setIn)
    {
        mIsAttachable = setIn;
    }

    bool Actor::isActive() const
    {
        return mIsActive;
    }

    bool Actor::isVisible() const
    {
        return mIsVisible;
    }

    bool Actor::isPaused() const
    {
        return mIsPaused;
    }

    bool Actor::isEditable() const
    {
        return mIsEditable;
    }

    bool Actor::isAttachable() const
    {
        return mIsAttachable;
    }

    void Actor::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        if (mIsActive)
        {
            if (!mIsPaused)
            {
                onUpdate(delta);
            }

            for(UINT32 i = 0; i < mChildren.getSize(); i++)
            {
                mChildren.get(i)->process(delta, rootTransformation * mTransformation);
            }
        }
    }

    void Actor::start()
    {
        onStart();

        for(UINT32 i = 0; i < mChildren.getSize(); i++)
        {
            mChildren.get(i)->start();
        }
    }

    void Actor::reset()
    {
        onReset();

        for(UINT32 i = 0; i < mChildren.getSize(); i++)
        {
            mChildren.get(i)->reset();
        }
    }

    void Actor::end()
    {
        onEnd();

        for(UINT32 i = 0; i < mChildren.getSize(); i++)
        {
            mChildren.get(i)->end();
        }
    }

} // namespace Berserk