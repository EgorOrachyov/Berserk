/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_COREREGISTERCLASSES_H
#define BERSERK_COREREGISTERCLASSES_H

namespace Berserk {

    class CoreRegisterClasses {
    public:

        /** Called once on Engine start-up to register all core Object-based classes */
        static void registerClasses();
    };

}




#endif //BERSERK_COREREGISTERCLASSES_H
