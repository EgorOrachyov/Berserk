/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_APPLICATION_HPP
#define BERSERK_APPLICATION_HPP

#include <core/Config.hpp>
#include <core/Engine.hpp>
#include <core/Typedefs.hpp>
#include <core/io/ArgumentParser.hpp>

#include <memory>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class Application
 * @brief Game application main class
 *
 * Application class is an entry point and main loop manager
 * for an end user application or a game. Controls overall
 * application and engine setup. Allows to customise various stages
 * of the application by overriding `On*` callback functions.
 *
 * Example of the usage in the game bellow:
 *
 * @code
 *  int main(int argc, const char* const *argv) {
 *   Application app;
 *   return app.Run(argc, argv);
 *  }
 * @endcode
 */
class Application {
public:
    BRK_API virtual ~Application() = default;

    /**
     * @brief Application entry point
     *
     * This function initializes application and engine system,
     * and enters main application loop of the game update.
     * This function blocks execution flow and returns only when
     * game is finished (or close of the application is requested).
     *
     * @param argc Input args count; must be passed from main
     * @param argv Input args list; must be passed from main
     *
     * @return Status code after execution; 0 on success
     */
    BRK_API int Run(int argc, const char *const *argv);

    /**
     * @brief On application initialize callback
     *
     * Called once, when the engine systems fully initialized.
     * Override this method in our application game to add
     * custom init logic to the engine and application.
     */
    BRK_API virtual void OnInitialize(){};

    /**
     * @brief On application finalize callback
     *
     * Called once, before the engine systems are finalized.
     * Override this method in our application game to add
     * custom init finalize to the engine and application.
     */
    BRK_API virtual void OnFinalize(){};

private:
    /** Global engine instance of the application */
    std::unique_ptr<Engine> gEngine;

    /** Input arguments */
    std::shared_ptr<ArgumentParser> gArgs;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_APPLICATION_HPP
