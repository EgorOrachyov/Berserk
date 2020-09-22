/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_THREAD_HPP
#define BERSERK_THREAD_HPP

#include <thread>

namespace Berserk {
    using Thread = std::thread;
    namespace ThisThread = std::this_thread;
}

#endif //BERSERK_THREAD_HPP