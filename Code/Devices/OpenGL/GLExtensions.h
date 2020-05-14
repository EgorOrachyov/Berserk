/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLEXTENSIONS_H
#define BERSERK_GLEXTENSIONS_H

namespace Berserk {

    class GLExtensions {
    public:

        bool extGetProgramBinary = false;

        static void init();
        static GLExtensions& getSingleton();

    private:
        static GLExtensions gExtensions;
    };


}




#endif //BERSERK_GLEXTENSIONS_H
