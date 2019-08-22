//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include <Resource/Resource.h>

namespace Berserk
{

    class ENGINE_API Material : public Resource
    {
    public:

        ~Material() override = default;



    protected:

        /** ID used for sorting on render queue */
        uint32 mMaterialID;

    };

} // namespace Berserk

#endif //BERSERK_MATERIAL_H