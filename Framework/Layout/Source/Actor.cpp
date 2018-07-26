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

    Actor::Actor(const CStaticString &name, const Matrix4x4f &transformation, FLOAT32 lifeTime) : Object(name)
    {
        mIsActive = true;
        mIsEditable = true;
        mIsPaused = false;
        mIsVisible = true;
        mIsAttachable = true;

        mLifeTime = lifeTime;

        mRoot = NULL;
        mChildren.init(2);    // ensure capacity for 4 elements

        mTransformation = transformation;
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

    void Actor::attachRoot(Actor *root)
    {
        if (root == NULL)
        {
            WARNING("Pass NULL root pinter. Attempt to create Head Root Actor");
            mRoot = NULL;
            mTransformation = newMatrix(1);
        }
        else
        {
            mRoot = root;
        }
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

} // namespace Berserk