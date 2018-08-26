//
// Created by Egor Orachyov on 26.08.2018.
//

#ifndef BERSERKENGINE_GLOBALLIGHT_H
#define BERSERKENGINE_GLOBALLIGHT_H

#include "DirectionalLight.h"

namespace Berserk
{

    class GlobalLight : public DirectionalLight
    {
    public:

        GlobalLight(const CStaticString& name, FLOAT32 lifeTime);
        virtual ~GlobalLight() = default;



    };

} // namespace Berserk

#endif //BERSERKENGINE_GLOBALLIGHT_H