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

#include <core/Engine.hpp>

BRK_NS_BEGIN

Engine::~Engine() {
    // Release in reverse order
    mEventDispatcher.reset();
    mScheduler.reset();
    mFileSystem.reset();

    // Remove global instance
    Remove(this);
}

FileSystem &Engine::GetFileSystem() {
    return *mFileSystem;
}

Scheduler &Engine::GetScheduler() {
    return *mScheduler;
}

EventDispatcher &Engine::GetEventDispatcher() {
    return *mEventDispatcher;
}

std::thread::id Engine::GetGameThreadId() const {
    return mGameThreadID;
}

void Engine::Init() {
    mGameThreadID = std::this_thread::get_id();
    mFileSystem = std::unique_ptr<FileSystem>(new FileSystem());
    mScheduler = std::unique_ptr<Scheduler>(new Scheduler());
    mEventDispatcher = std::unique_ptr<EventDispatcher>(new EventDispatcher());

    // Provide singleton
    Provide(this);
}

void Engine::Configure() {
}

void Engine::Update(float dt) {
    mScheduler->Update(dt);
    mEventDispatcher->Update();
}

BRK_NS_END