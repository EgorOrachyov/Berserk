//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_LAYOUTCLASSTESTING_H
#define BERSERKENGINE_LAYOUTCLASSTESTING_H

#include "Objects/Actor.h"
#include "Managers/ObjectManager.h"

#include "Strings/UtilityString.h"

#include "Objects/Cameras/Camera.h"

void ActorProcessTesting()
{
    using namespace Berserk;

    Actor root(CNAME("RootActor"));
    Actor someactor1(CNAME("Object1"));
    Actor someactor2(CNAME("Object2"));
    Actor someactor3(CNAME("Object3"));

    root.addMovement(Vector3f(1,0,0));
    someactor1.addMovement(Vector3f(0,1,0));
    someactor3.addMovement(Vector3f(0,0,1));

    root.attachActor(&someactor1);
    root.attachActor(&someactor2);
    someactor1.attachActor(&someactor3);

    //root.process(0.5, newMatrix(1.0));

    Matrix4x4f absolute = someactor3.getAbsoluteTransformation();

    printf("%f %f %f \n", absolute.m[3], absolute.m[7], absolute.m[11]);
}

void ObjectManagerTesting()
{
    using namespace Berserk;

    Actor actor(CNAME("NewActor"));
    Actor child(CNAME("NewChild"));
    actor.attachActor(&child);

    ObjectManager manager;
    manager.init();

    manager.add(&actor);
    manager.add(&child);

    Actor* toGet = (Actor*)manager.get(CNAME("NewActor"));

    if (toGet == NULL)
    {
        PUSH("Cannot get actor");
    }
    else
    {
        PUSH("Got actor with name %s", toGet->getName().getChars());
    }

    toGet = (Actor*)manager.get(CNAME("NewChild"));

    if (toGet == NULL)
    {
        PUSH("Cannot get actor");
    }
    else
    {
        PUSH("Got actor with name %s", toGet->getName().getChars());
    }

}

void InheritanceTesting()
{
    using namespace Berserk;

    Camera camera(CNAME("MainCamera"));

}

#endif //BERSERKENGINE_LAYOUTCLASSTESTING_H
