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

#ifndef BERSERK_EVENTDISPATCHER_HPP
#define BERSERK_EVENTDISPATCHER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/event/Event.hpp>

#include <functional>
#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class EventDispatcher
 * @brief Event dispatcher is responsible for managing engine events
 *
 * Use this class to:
 *  - Register events
 *  - Subscribe listeners to events
 *  - Dispatch events and notify listeners
 *
 *  Event dispatcher provides handlers for subscribed functions,
 *  so parameters and execution can be tweaked after subscription.
 */
class EventDispatcher final {
public:
    /** Function type to subscribe listener */
    using ListenerFunc = std::function<bool(const Event &)>;
    /** Handle to identify subscribed listener function entry */
    using Handle = size_t;

    /**
     * @brief Dispatch new event to listeners.
     *
     * Dispatches provided event to subscribed listeners of this event type.
     * Event type if defined from event overridden `Event::GetEventType()` function.
     * Event is not dispatched immediately, instead it is queued to be dispatched later.
     * It is safe to dispatch events inside listeners, while responding on incoming event.
     *
     * @note Must be called from game thread
     *
     * @param event Event to dispatch; must be not null
     */
    BRK_API void Dispatch(const Ref<Event> &event);

    /**
     * @brief Subscribe listener function to specified event type
     *
     * The listener function will be called for each new dispatched event
     * of specified type `eventType`. If listener is set paused, then
     * it won't receive events, until it is resumed. Listener is identified
     * by returned `handle`. Use this handle further to tweak subscribed listener.
     *
     * @note Must be called from game thread
     *
     * @param eventType Type of the event to listen to
     * @param func Function of the listener to call on event
     * @param paused Pass true if paused; not accept events until paused is set to false
     *
     * @return Handle of subscribed listener
     */
    BRK_API Handle Subscribe(const EventType &eventType, ListenerFunc func, bool paused = false);

    /**
     * @brief Unsubscribe listener associated with specified handle
     *
     * @note Must be called from game thread
     *
     * @param handle Handle of subscribed listener
     */
    BRK_API void Unsubscribe(Handle handle);

    /**
     * @brief Pause listening for events of specified listener by handle
     *
     * @note Must be called from game thread
     *
     * @param handle Handle of subscribed listener
     */
    BRK_API void Pause(Handle handle);

    /**
     * @brief Resume listening for events of specified listener by handle
     *
     * @note Must be called from game thread
     *
     * @param handle Handle of subscribed listener
     */
    BRK_API void Resume(Handle handle);

private:
    friend class Engine;

    /** Listener of concrete event type */
    struct Listener {
        ListenerFunc callback;
        Handle handle;
        bool paused = false;
    };

    void Update();
    Handle GetNext();

private:
    /** Map of event types and subscribed listeners */
    std::unordered_map<EventType, std::vector<Listener>> mListeners;
    /** Type of event mapping for each listener  */
    std::unordered_map<Handle, EventType> mListenersTypes;

    /** Listeners pending to be added */
    std::vector<Listener> mPendingAdd;
    /** Listeners pending to be removed */
    std::vector<Handle> mPendingRemove;
    /** Modify listener state */
    std::vector<std::pair<Handle, bool>> mPendingPause;

    /** Events to be processed */
    std::vector<Ref<Event>> mQueuedEvents;
    std::vector<Ref<Event>> mQueuedEventsExec;

    /** Handle of the next listener to subscribe */
    Handle mNextHandle = 0;
};

/**
 * @}
 */

BRK_NS_END


#endif//BERSERK_EVENTDISPATCHER_HPP
