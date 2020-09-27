/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Application.hpp>

namespace Berserk {
    Application* Application::gApplication = nullptr;

    Application::Application() {
        if (gApplication) {
            // Allowed only single app instance
            return;
        }

        gApplication = this;
    }

    Application& Application::GetSingleton() {
        return *gApplication;
    }

}