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

#ifndef BERSERK_ENGINE_HPP
#define BERSERK_ENGINE_HPP

#include <core/Config.hpp>
#include <core/EventDispatcher.hpp>
#include <core/Scheduler.hpp>
#include <core/input/Input.hpp>
#include <platform/FileSystem.hpp>
#include <platform/Output.hpp>
#include <platform/WindowManager.hpp>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Engine
 * @brief Root manager class
 *
 * Engine is singleton class which is responsible for core
 * engine infrastructure setup, provides access to core
 * managers, maintenance main loop update and controls execution.
 *
 * Standard way to access Engine is `Engine::Instance()`.
 *
 * @details
 *      Engine responsible for:
 *      - Core engine managers initialization and access
 *      - Engine main loop update
 *      - Primary window and graphic configuration
 */
class Engine final {
public:
    BRK_API Engine() = default;
    BRK_API ~Engine();

    /**
     * @brief Request close of the application
     * Call this function to request close and application shut-down.
     */
    BRK_API void RequestClose();

    /**
     * @brief Check if close of the application is requested
     * @return True if application close requested
     */
    BRK_API bool CloseRequested();

    /** @return Engine standard output */
    BRK_API Output &GetOutput();

    /** @return Engine file system utils */
    BRK_API FileSystem &GetFileSystem();

    /** @return Engine scheduler instance for frame/timer actions */
    BRK_API Scheduler &GetScheduler();

    /** @return Engine event dispatch instance for events management */
    BRK_API EventDispatcher &GetEventDispatcher();

    /** @return Engine windows manager class */
    BRK_API WindowManager &GetWindowManager();

    /** @return Engine input class */
    BRK_API Input &GetInput();

    /** @return Game thread id */
    BRK_API std::thread::id GetGameThreadId() const;

    /** @brief Return engine global instance */
    BRK_API static Engine &Instance();

private:
    friend class Application;

    void Init();
    void Configure();
    void SetWindowManager(std::shared_ptr<WindowManager> windowManager);
    void SetInput(std::shared_ptr<Input> input);
    void Update(float dt);

private:
    /** Engine standard output */
    std::unique_ptr<Output> mOutput;

    /** Engine file system utils */
    std::unique_ptr<FileSystem> mFileSystem;

    /** Engine scheduler for frame/timer events */
    std::unique_ptr<Scheduler> mScheduler;

    /** Engine event dispatch instance for events management */
    std::unique_ptr<EventDispatcher> mEventDispatcher;

    /** Engine windows manager class */
    std::shared_ptr<WindowManager> mWindowManager;

    /** Engine input manager */
    std::shared_ptr<Input> mInput;

    /** Main game thread id */
    std::thread::id mGameThreadID;

    /** Close request */
    std::atomic_bool mCloseRequested{false};

    /** Global instance */
    static Engine *gEngine;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_ENGINE_HPP
