/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORMSETUP_HPP
#define BERSERK_PLATFORMSETUP_HPP

namespace Berserk {

    class PlatformSetup {
    public:
        PlatformSetup();
        ~PlatformSetup();
        void FixedUpdate();
    };

}

#define BERSERK_DEFINE_FIXTURE(Class) \
    class Class : public ::testing::Test, public ::Berserk::PlatformSetup { };


#endif //BERSERK_PLATFORMSETUP_HPP