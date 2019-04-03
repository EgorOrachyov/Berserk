//
// Created by Egor Orachyov on 02.04.2019.
//

#ifndef BERSERK_IFACTORYINITIALIZER_H
#define BERSERK_IFACTORYINITIALIZER_H

#include <Foundation/IObjectInitializer.h>

namespace Berserk::EntitySystem
{

    class IFactoryInitializer
    {
    public:

        explicit IFactoryInitializer(IObjectInitializer* initializer) : mObjectInitializer(initializer) {}

        IObjectInitializer* getObjectInitializer() const { return mObjectInitializer; }

    private:

        IObjectInitializer* mObjectInitializer;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IFACTORYINITIALIZER_H