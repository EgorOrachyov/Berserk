//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_LAYOUTCLASSTESTING_H
#define BERSERKENGINE_LAYOUTCLASSTESTING_H

#include "Objects/Actor.h"
#include "Strings/UtilityString.h"
#include "Math/UtilityMatrices.h"

void ActorProcessTesting()
{
    using namespace Berserk;

    Actor root(CNAME("RootActor"));
    Actor someactor1(CNAME("Object1"));
    Actor someactor2(CNAME("Object2"));
    Actor someactor3(CNAME("Object3"));

    root.attachActor(&someactor1);
    root.attachActor(&someactor2);
    someactor1.attachActor(&someactor3);

    root.process(0.5, newMatrix(1.0));
}

#endif //BERSERKENGINE_LAYOUTCLASSTESTING_H
