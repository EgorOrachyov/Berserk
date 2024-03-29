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

#ifndef BERSERK_EVENT_HPP
#define BERSERK_EVENT_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/RcPtr.hpp>
#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    /**
     * @brief Event handler
     *
     * Provides safe event subscription handler for managing connections to various events.
     * Allows to track and share hnd, and disconnect event if needed.
     */
    class EventHnd {
    public:
        EventHnd() = default;

        EventHnd(const EventHnd& other) {
            mConnectionId = other.mConnectionId;
            mData = other.mData;

            if (IsConnected()) {
                mData->AddConnectionRef(mConnectionId);
            }
        };

        EventHnd(EventHnd&& other) noexcept {
            mConnectionId = other.mConnectionId;
            mData = std::move(other.mData);

            other.mConnectionId = nullptr;
            other.mData = nullptr;
        };

        ~EventHnd() {
            if (mData.IsNotNull()) {
                mData->ReleaseConnectionRef(mConnectionId);
                mData = nullptr;
                mConnectionId = nullptr;
            }
        }

        EventHnd& operator=(const EventHnd& other) {
            if (this == &other)
                return *this;

            this->~EventHnd();
            new (this) EventHnd(other);
            return *this;
        }

        EventHnd& operator=(EventHnd&& other) noexcept {
            if (this == &other)
                return *this;

            this->~EventHnd();
            new (this) EventHnd(std::move(other));
            return *this;
        }

        /** Fully disconnects this connection to the event */
        void Disconnect() {
            if (mData.IsNotNull()) {
                mData->Disconnect(mConnectionId);
                mData = nullptr;
                mConnectionId = nullptr;
            }
        }

        /** @return True if this hnd connected to the event and receives callbacks */
        bool IsConnected() const {
            return mData.IsNotNull() && mData->IsConnected(mConnectionId);
        }

    private:
        template<typename ... TArgs>
        friend class EventPublisher;

        template<typename ... TArgs>
        friend class Event;

        class Connection;

        /** Provides connections routines */
        class Provider: public RefCountedThreadSafe {
        public:
            ~Provider() override = default;
            virtual void ReleaseConnectionRef(Connection* connectionId) = 0;
            virtual void AddConnectionRef(Connection* connectionId) = 0;
            virtual void Disconnect(Connection* connectionId) = 0;
            virtual bool IsConnected(Connection* connectionId) const = 0;
        };

        /** Called only by event provider */
        explicit EventHnd(RcPtr<Provider>&& data, Connection* connectionId)
            : mData(std::move(data)), mConnectionId(connectionId) {

        }

        /** Shared event data for safe event hnd */
        RcPtr<Provider> mData;
        /** ID of this event connection */
        Connection* mConnectionId = nullptr;
    };


    /**
     * @brief Event Publisher
     *
     * Allows you to register method callbacks that get notified when the event is triggered.
     * Event provide safe hnd to the registered connection, that allows you to track your connection.
     *
     * If connection has no active handlers, it will be removed from the event subscription list.
     *
     * @note This event publisher also allows you to trigger events. It must be stored privately and
     *       do not shown outside the usage scope. To provide users ability to subscribe -
     *       create public field of type Event<...>.
     *
     * @tparam TArgs Types of the arguments dispatched to the subscribers within event.
     */
    template<typename ... TArgs>
    class EventPublisher {
    public:
        EventPublisher() : mData(InternalData::Create()) {

        }

        ~EventPublisher() = default;

        /**
         * Subscribes listener function to this event.
         *
         * @tparam Callback User callback type
         * @param callback Callback to be called on this event
         *
         * @return Handler of this event connection
         */
        template<typename Callback>
        EventHnd Subscribe(Callback&& callback) {
            return EventHnd((RcPtr<EventHnd::Provider>) mData, (EventHnd::Connection*) mData->Subscribe(std::forward<Callback>(callback)));
        }

        /**
         * Dispatch event to subscribers withs specified args
         * @param args Args of the event to dispatch
         */
        void Dispatch(TArgs&& ... args) {
            if (!IsEmpty()) {
                RcPtr<InternalData> copy = mData;
                copy->Dispatch(std::forward<TArgs>(args)...);
            }
        }

        /** Release all connections to this event */
        void Clear() {
            mData->Clear();
        }

        /** @return Number of active connections to this event */
        uint32 GetConnectionsCount() const {
            return mData.IsNotNull()? mData->GetActiveConnectionsCount(): 0;
        }

        /** @return True if this event has no connections */
        bool IsEmpty() const {
            return mData.IsNull() || !mData->HasConnections();
        }

    private:
        using CallbackType = Function<void(TArgs ...)>;

        template<typename ... BArgs>
        friend class Event;

        class Connection {
        public:
            explicit Connection(CallbackType&& func)
                : func(std::move(func)) {

            }

            /** Action to perform */
            CallbackType func;
            /** Refs count to this connection */
            uint64 refsCount = 1;
            /** Links */
            Connection* next = nullptr;
            Connection* prev = nullptr;
            /** True if this func must be called on event dispatch */
            bool isActive = true;
        };

        class InternalData: public EventHnd::Provider {
        public:

            InternalData()
                : mAllocator(POOL_CHUNK_SIZE) {

            }

            ~InternalData() override {
                assert(mFirstActive == nullptr);
                assert(mLastActive == nullptr);
                assert(mPending.GetSize() == 0);
                assert(mActiveConnectionsCount == 0);
            };

            void Dispatch(TArgs&& ... args) {
                Guard<RecursiveMutex> guard(mMutex);

                auto prevState = mIsCurrentlyTriggered;
                mIsCurrentlyTriggered = true; // Now in recursive dispatch we will know that we already here

                auto current = mFirstActive;
                while (current != nullptr) {
                    if (current->isActive) {
                        // If was not added to remove or unlink
                        current->func(std::forward<TArgs>(args)...);
                    }

                    current = current->next;
                }

                mIsCurrentlyTriggered = prevState;

                if (!mIsCurrentlyTriggered) {
                    for (auto action: mPending) {
                        switch (action.pendingOp) {
                            case PendingOp::Connect:
                                Connect(action.connection);
                                break;
                            case PendingOp::Remove:
                                Remove(action.connection);
                                break;
                            case PendingOp::Unlink:
                                Unlink(action.connection);
                                break;
                            default:
                                break;
                        }
                    }

                    mPending.Clear();
                }
            }

            template<typename Callback>
            Connection* Subscribe(Callback&& callback) {
                Guard<RecursiveMutex> guard(mMutex);

                Function<void(TArgs ... )> function = std::forward<Callback>(callback);

                Connection* connection = AllocateConnection();
                new (connection) Connection(std::move(function));

                if (mIsCurrentlyTriggered)
                    mPending.Emplace(PendingOp::Connect, connection);
                else
                    Connect(connection);

                return connection;
            }

            void ReleaseConnectionRef(EventHnd::Connection* connectionId) override {
                Guard<RecursiveMutex> guard(mMutex);

                auto connection = (Connection*) connectionId;
                connection->refsCount -= 1;

                if (connection->refsCount == 0) {
                    mActiveConnectionsCount -= connection->isActive? 1: 0;

                    if (mIsCurrentlyTriggered)
                       mPending.Emplace(PendingOp::Remove, connection);
                    else
                        Remove(connection);
                }
            }

            void AddConnectionRef(EventHnd::Connection* connectionId) override {
                Guard<RecursiveMutex> guard(mMutex);
                auto connection = (Connection*) connectionId;
                connection->refsCount += 1;
            }

            void Disconnect(EventHnd::Connection* connectionId) override {
                Guard<RecursiveMutex> guard(mMutex);

                auto connection = (Connection*) connectionId;
                connection->refsCount -= 1;
                mActiveConnectionsCount -= connection->isActive? 1: 0;

                if (connection->refsCount == 0) {
                    connection->isActive = false;

                    if (mIsCurrentlyTriggered)
                        mPending.Emplace(PendingOp::Remove, connection);
                    else
                        Remove(connection);
                }
                else if (connection->isActive) {
                    connection->isActive = false;
                    connection->func = CallbackType();

                    if (mIsCurrentlyTriggered)
                        mPending.Emplace(PendingOp::Unlink, connection);
                    else
                        Unlink(connection);
                }
            }

            bool IsConnected(EventHnd::Connection* connectionId) const override {
                Guard<RecursiveMutex> guard(mMutex);
                auto connection = (Connection*) connectionId;
                return connection->isActive;
            }

            uint32 GetActiveConnectionsCount() const {
                Guard<RecursiveMutex> guard(mMutex);
                return mActiveConnectionsCount;
            }

            bool HasConnections() const {
                Guard<RecursiveMutex> guard(mMutex);
                return mActiveConnectionsCount > 0;
            }

            void Clear() {
                Guard<RecursiveMutex> guard(mMutex);

                if (mIsCurrentlyTriggered) {
                    auto current = mFirstActive;
                    while (current != nullptr) {
                        if (current->isActive) {
                            current->isActive = false;
                            mActiveConnectionsCount -= 1;
                            mPending.Emplace(PendingOp::Unlink, current);
                        }

                        current = current->next;
                    }
                }
                else {
                    auto current = mFirstActive;
                    while (current != nullptr) {
                        auto next = current->next;

                        if (current->isActive) {
                            current->isActive = false;
                            mActiveConnectionsCount -= 1;
                            Unlink(current);
                        }

                        current = next;
                    }
                }
            }

            static RcPtr<InternalData> Create() {
                return RcPtr<InternalData>(Memory::Make<InternalData>());
            }

        protected:

            void Unlink(Connection* connection) {
                if (connection->prev)
                    connection->prev->next = connection->next;

                if (connection->next)
                    connection->next->prev = connection->prev;

                if (mFirstActive == connection)
                    mFirstActive = connection->next;

                if (mLastActive == connection)
                    mLastActive = connection->prev;
            }

            void Remove(Connection* connection) {
                Unlink(connection);
                connection->~Connection();
                mAllocator.Deallocate(connection);
            }

            void Connect(Connection* connection) {
                connection->next = nullptr;
                connection->prev = mLastActive;

                if (mLastActive)
                    mLastActive->next = connection;

                mLastActive = connection;

                if (mFirstActive == nullptr)
                    mFirstActive = connection;

                mActiveConnectionsCount += 1;
            }

            Connection* AllocateConnection() {
                return (Connection*) mAllocator.Allocate(sizeof(Connection));
            }

        private:

            enum class PendingOp {
                Connect,
                Remove,
                Unlink
            };

            struct Pending {
                Pending(PendingOp pendingOp, Connection* connection)
                    : pendingOp(pendingOp), connection(connection) {

                }

                PendingOp pendingOp;
                Connection* connection;
            };

            static const uint64 POOL_CHUNK_SIZE = sizeof(Connection);

            PoolAllocator<> mAllocator;
            Array<Pending> mPending;
            Connection* mFirstActive = nullptr;
            Connection* mLastActive = nullptr;
            uint32 mActiveConnectionsCount = 0;
            bool mIsCurrentlyTriggered = false;

            mutable RecursiveMutex mMutex;
        };

        /** Event data */
        RcPtr<InternalData> mData;
    };

    /**
     * @brief Event
     *
     * Events allows you to register method callbacks that get notified when the event is triggered.
     * Event provide safe hnd to the registered connection, that allows you to track your connection.
     *
     * If connection has no active handlers, it will be removed from the event subscription list.
     *
     * @tparam TArgs Types of the arguments dispatched to the subscribers within event.
     */
    template <typename ... TArgs>
    class Event {
    public:
        explicit Event(const EventPublisher<TArgs...> &event) : mData(event.mData) {}

        Event() = default;
        Event(const Event& other) = default;
        Event(Event&& other) noexcept = default;
        ~Event() = default;

        Event& operator=(const Event& other) = default;
        Event& operator=(Event&& other) noexcept = default;

        /**
         * Subscribes listener function to this event.
         *
         * @tparam Callback User callback type
         * @param callback Callback to be called on this event
         *
         * @return Handler of this event connection
         */
        template<typename Callback>
        EventHnd Subscribe(Callback&& callback) {
            if (mData.IsNotNull()) {
                return EventHnd((RcPtr<EventHnd::Provider>) mData, (EventHnd::Connection*) mData->Subscribe(std::forward<Callback>(callback)));
            }

            return EventHnd();
        }

    private:
        using Super = EventPublisher<TArgs...>;

        /** Event data */
        RcPtr<typename Super::InternalData> mData;
    };

}

#endif //BERSERK_EVENT_HPP