//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_MATERIAL_H
#define BERSERKENGINE_MATERIAL_H

#include "Objects/Object.h"

namespace Berserk
{

    class Material : public Object
    {
    public:

        Material(const CStaticString &name) : Object(name) {};
        virtual ~Material() = default;

    protected:

        virtual void apply() = 0;
    };

} // namespace Berserk




#endif //BERSERKENGINE_MATERIAL_H
