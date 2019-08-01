//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_OBJECT_H
#define BERSERK_OBJECT_H

#include <Object/Allocatable.h>
#include <Object/Reflectable.h>

namespace Berserk
{

    class Object : public Allocatable, public Reflectable<Object>
    {
    public:

        Object(int id) {

        }

    };

} // namespace Berserk

#endif //BERSERK_OBJECT_H