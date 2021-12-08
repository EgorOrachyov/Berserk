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

#include <core/EventDispatcher.hpp>
#include <core/io/Logger.hpp>

#include <algorithm>
#include <cassert>

BRK_NS_BEGIN

#define BRK_EVENT_DISPATCHER_FIND_LISTENER(handle)                                                                             \
    auto query = mListenersTypes.find(handle);                                                                                 \
                                                                                                                               \
    if (query == mListenersTypes.end()) {                                                                                      \
        BRK_ERROR("No such listener with handle=" << (handle));                                                                \
        continue;                                                                                                              \
    }                                                                                                                          \
                                                                                                                               \
    const auto &eventType = query->second;                                                                                     \
    auto &listeners = mListeners[eventType];                                                                                   \
    auto listener = std::find_if(listeners.begin(), listeners.end(), [=](const Listener &l) { return l.handle == (handle); }); \
                                                                                                                               \
    if (listener == listeners.end()) {                                                                                         \
        BRK_ERROR("No such listener with handle=" << (handle));                                                                \
        continue;                                                                                                              \
    }

void EventDispatcher::Dispatch(const Ref<Event> &event) {
    if (event.IsNull()) {
        BRK_ERROR("Cannot dispatch null event");
        return;
    }

    mQueuedEvents.push_back(event);
}

EventDispatcher::Handle EventDispatcher::Subscribe(const EventType &eventType, EventDispatcher::ListenerFunc func, bool paused) {
    auto handle = GetNext();

    Listener listener;
    listener.callback = std::move(func);
    listener.handle = handle;
    listener.paused = paused;

    mPendingAdd.push_back(std::move(listener));
    mListenersTypes.emplace(handle, eventType);

    return handle;
}

void EventDispatcher::Unsubscribe(EventDispatcher::Handle handle) {
    mPendingRemove.push_back(handle);
}

void EventDispatcher::Pause(EventDispatcher::Handle handle) {
    mPendingPause.emplace_back(handle, true);
}

void EventDispatcher::Resume(EventDispatcher::Handle handle) {
    mPendingPause.emplace_back(handle, false);
}

void EventDispatcher::Update() {
    // Add pending listeners
    for (auto &listener : mPendingAdd) {
        auto query = mListenersTypes.find(listener.handle);
        assert(query != mListenersTypes.end());
        const auto &eventType = query->second;
        auto &listeners = mListeners[eventType];
        listeners.push_back(std::move(listener));
    }
    mPendingAdd.clear();

    // Update state
    for (const auto &pending : mPendingPause) {
        auto handle = pending.first;
        BRK_EVENT_DISPATCHER_FIND_LISTENER(handle);
        listener->paused = pending.second;
    }
    mPendingPause.clear();

    // Remove pending listeners
    for (auto handle : mPendingRemove) {
        BRK_EVENT_DISPATCHER_FIND_LISTENER(handle);
        mListenersTypes.erase(listener->handle);
        listeners.erase(listener);
    }
    mPendingRemove.clear();

    // Dispatch actual events
    std::swap(mQueuedEvents, mQueuedEventsExec);
    for (const auto &event : mQueuedEvents) {
        const auto &eventType = event->GetEventType();
        auto query = mListeners.find(eventType);

        // There may be no listeners for this event
        if (query != mListeners.end()) {
            for (auto &listener : query->second) {
                // Check if not paused and call, if returns true - discard processing
                // and go to the next event in the queue
                if (!listener.paused && listener.callback(*event))
                    break;
            }
        }
    }
    mQueuedEventsExec.clear();
}

EventDispatcher::Handle EventDispatcher::GetNext() {
    return mNextHandle++;
}

#undef BRK_EVENT_DISPATCHER_FIND_LISTENER

BRK_NS_END
