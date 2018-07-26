//
// Created by Egor Orachyov on 25.07.2018.
//

#ifndef BERSERKENGINE_ACTOR_H
#define BERSERKENGINE_ACTOR_H

#include "Objects/Object.h"
#include "Math/Matrix4x4f.h"
#include "Containers/ArrayList.h"

namespace Berserk
{

    class Actor : public Object
    {
    public:

        Actor(const CStaticString& name, FLOAT32 lifeTime = 0);
        virtual ~Actor();

        virtual void onBegin();
        virtual void onUpdate(FLOAT64 elapsedTime);
        virtual void onReset();
        virtual void onEnd();

        void attachActor(Actor* actor);

        const Actor* getRoot() const;
        const ArrayList<Actor*> &getChildren() const;
        const Matrix4x4f &getTransformation() const;
        const Matrix4x4f &getAbsoluteTransformation() const;

        void setTransformation(const Matrix4x4f &transformation);
        void addMovement(const Vector3f &translation);
        void addRotation(const Vector3f &axis, FLOAT32 angle);
        void addScale(FLOAT32 factor);

        void setActive(bool setIn = true);
        void setVisible(bool setIn = true);
        void setPaused(bool setIn = true);
        void setEditable(bool setIn = true);
        void setAttachable(bool setIn = true);

        bool isActive() const;
        bool isVisible() const;
        bool isPaused() const;
        bool isEditable() const;
        bool isAttachable() const;

    protected:

        virtual void process(FLOAT64 delta, const Matrix4x4f &rootTransformation);
        virtual void start();
        virtual void reset();
        virtual void end();

    protected:

        INT8 mIsActive      : 1;
        INT8 mIsVisible     : 1;
        INT8 mIsPaused      : 1;
        INT8 mIsEditable    : 1;
        INT8 mIsAttachable  : 1;

        FLOAT32 mLifeTime;

        Actor* mRoot;
        ArrayList<Actor*> mChildren;
        Matrix4x4f mTransformation;

    };

} // namespace Berserk

#endif //BERSERKENGINE_ACTOR_H