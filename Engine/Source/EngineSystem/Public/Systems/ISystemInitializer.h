//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_ISYSTEMINITIALIZER_H
#define BERSERK_ISYSTEMINITIALIZER_H

#include <Foundation/IObjectInitializer.h>

namespace Berserk::Engine
{

    class ISystemInitializer : public IObjectInitializer
    {
    public:

        /** Default system initializer from string name */
        explicit ISystemInitializer(const char *name, IAllocator *general = nullptr)
                : IObjectInitializer(name, general)
        {

        }

        ~ISystemInitializer() override = default;

    };

} // namespace Berserk::Engine

#endif //BERSERK_ISYSTEMINITIALIZER_H