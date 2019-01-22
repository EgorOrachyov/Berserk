//
// Created by Egor Orachyov on 25.07.2018.
//

#ifndef BERSERKENGINE_ACTOR_H
#define BERSERKENGINE_ACTOR_H

#include "Objects/Object.h"
#include "Math/Matrix4x4f.h"
#include "Containers/ArrayList.h"

#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    class ENGINE_EXPORT Actor : public Object
    {
    public:

        Actor(const CStaticString& name, FLOAT32 lifeTime = 0);
        virtual ~Actor();

        virtual void onInit();
        virtual void onUpdate(FLOAT64 elapsedTime);
        virtual void onReset();
        virtual void onDestroy();

        void attachActor(Actor* actor);

        const Actor* getRoot() const;
        const ArrayList<Actor*> &getChildren() const;
        const Matrix4x4f &getTransformation() const;
        const Matrix4x4f &getWorldTransformation() const;

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

        friend class Scene;

        virtual void process(FLOAT64 delta, const Matrix4x4f &rootTransformation);
        virtual void init();
        virtual void reset();
        virtual void destroy();

        void processActor(FLOAT64 delta, const Matrix4x4f &rootTransformation);

    protected:

        FLOAT32 mLifeTime;

        Actor* mRoot;
        ArrayList<Actor*> mChildren;
        Matrix4x4f mTransformation;
        Matrix4x4f mWorldTransformation;

    };

} // namespace Berserk

#endif //BERSERKENGINE_ACTOR_H