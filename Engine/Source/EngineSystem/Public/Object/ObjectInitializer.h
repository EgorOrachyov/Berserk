//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_OBJECTINITIALIZER_H
#define BERSERK_OBJECTINITIALIZER_H

#include <Engine/IEnvironment.h>

namespace Berserk
{

    /** Stores base data needed to initialize game object */
    class ENGINE_API ObjectInitializer
    {
    public:

        /** Data constructor */
        explicit ObjectInitializer(IEnvironment& environment)
            : mEnvironment(environment)
        {

        }

        /** @return Engine environment */
        IEnvironment& getEnvironment() const { return mEnvironment; }

    protected:

        IEnvironment& mEnvironment;

    };

} // namespace Berserk

#endif //BERSERK_OBJECTINITIALIZER_H