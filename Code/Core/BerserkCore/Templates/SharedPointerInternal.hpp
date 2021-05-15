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

#ifndef BERSERK_SHAREDPOINTERINTERNAL_HPP
#define BERSERK_SHAREDPOINTERINTERNAL_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/Atomic.hpp>

namespace Berserk {

    /** Shared and weak references storage */
    class ReferenceControllerBase {
    public:
        AtomicUint32 sharedReferences{0};
        AtomicUint32 weakReferences{0};

        explicit ReferenceControllerBase() = default;
        virtual ~ReferenceControllerBase() = default;
        virtual void DestroyObject() = 0;

        ReferenceControllerBase& operator=(const ReferenceControllerBase& other) = delete;
        ReferenceControllerBase& operator=(ReferenceControllerBase&& other) = delete;
    };

    /** Reference count for objects with custom delete logic */
    template<typename Object, typename Deleter>
    class ReferenceControllerWithDeleter: public Deleter, public ReferenceControllerBase {
    public:
        ReferenceControllerWithDeleter(Object* object, Deleter&& deleter)
            : Deleter(std::move(deleter)),
              mObject(object) {
        }
        ~ReferenceControllerWithDeleter() override = default;

        void DestroyObject() override {
            auto& d = static_cast<Deleter&>(*this);
            d(mObject);
        }

    private:
        // For objects without virtual destructor
        Object* mObject;
    };

    /** Reference count for objects, stored inside ref count structure */
    template<typename Object>
    class ReferenceControllerWithObject: public ReferenceControllerBase {
    public:
        ReferenceControllerWithObject() = default;

        explicit ReferenceControllerWithObject(Object&& object) {
            new (Get()) Object(std::move(object));
        }

        ~ReferenceControllerWithObject() override = default;

        void DestroyObject() override {
            // No deallocation
            Get()->~Object();
        }

        Object* Get() {
            return (Object*)mObject;
        }

    private:
        // For objects without virtual destructor
        char mObject[sizeof(Object)] {};
    };

    /** Default deleter for objects, created with default global allocator */
    template <typename Object>
    struct DefaultDeleter {
        void operator()(Object* toDelete) {
            Memory::Release(toDelete);
        }
    };

    template <typename Object>
    using DefaultRefController = ReferenceControllerWithDeleter<Object, DefaultDeleter<Object>>;

    class ReferenceControllerOps {
    public:

        void RawSet(ReferenceControllerBase* base) {
            assert(mRefController == nullptr);
            mRefController = base;
        }

        uint32 GetSharedRefs() const {
            assert(mRefController);
            return mRefController->sharedReferences.load();
        }

        uint32 GetWeakRefs() const {
            assert(mRefController);
            return mRefController->weakReferences.load();
        }

        uint32 AddSharedRef() const {
            assert(mRefController);
            return mRefController->sharedReferences.fetch_add(1);
        }

        bool TryAddSharedRef() const {
            assert(mRefController);

            uint32 current = mRefController->sharedReferences.load();
            bool changed = false;

            while (!changed && current > 0) {
                changed = mRefController->sharedReferences.compare_exchange_strong(current, current + 1);
            }

            return changed;
        }

        uint32 AddWeakRef() const {
            assert(mRefController);
            return mRefController->weakReferences.fetch_add(1);
        }

        uint32 ReleaseSharedRef() const {
            assert(mRefController);
            auto count = mRefController->sharedReferences.fetch_sub(1);

            if (count == 1) {
                // If we were last: destroy object, but keep meta
                mRefController->DestroyObject();

                // Release one weak ref (this ref added syntactically by creator)
                ReleaseWeakRef();
            }

            return count;
        }

        uint32 ReleaseWeakRef() const {
            assert(mRefController);
            auto count = mRefController->weakReferences.fetch_sub(1);

            if (count == 1) {
                // If we were last: destroy object, but keep meta
                Memory::Release(mRefController);
                mRefController = nullptr;
            }

            return count;
        }

        bool IsEmpty() const {
            return mRefController == nullptr;
        }

        bool IsNotEmpty() const {
            return mRefController != nullptr;
        }


    protected:
        mutable ReferenceControllerBase* mRefController = nullptr;
    };

    class SharedRefController: public ReferenceControllerOps {
    public:
        using ReferenceControllerOps::mRefController;

        SharedRefController() = default;

        explicit SharedRefController(ReferenceControllerBase* base) {
            mRefController = base;

            if (mRefController) {
                // First shared ptr always adds additional weak reference
                // (so if last shared ptr released - it decrements also weak reference and checks,
                // if can release ref count, else last real weak ref will cause release of the ref count)
                AddSharedRef();
                AddWeakRef();
            }
        }

        SharedRefController(const SharedRefController& other)  {
            mRefController = other.mRefController;

            if (mRefController) {
                AddSharedRef();
            }
        }

        SharedRefController(SharedRefController&& other) noexcept {
            mRefController = other.mRefController;
            other.mRefController = nullptr;
        }

        ~SharedRefController() {
            if (mRefController) {
                ReleaseSharedRef();
                mRefController = nullptr;
            }
        }

        SharedRefController& operator=(const SharedRefController& other) {
            if (mRefController == other.mRefController)
                return *this;

            SharedRefController tmp(other);
            std::swap(*this, tmp);

            return *this;
        }

        SharedRefController& operator=(SharedRefController&& other) noexcept {
            if (mRefController == other.mRefController)
                return *this;

            this->~SharedRefController();
            mRefController = other.mRefController;
            other.mRefController = nullptr;

            return *this;
        }
    };

    class WeakRefController: public ReferenceControllerOps {
    public:
        using ReferenceControllerOps::mRefController;

        WeakRefController() = default;

        WeakRefController(const WeakRefController& other)  {
            mRefController = other.mRefController;

            if (mRefController) {
                AddWeakRef();
            }
        }

        WeakRefController(WeakRefController&& other) noexcept {
            mRefController = other.mRefController;
            other.mRefController = nullptr;
        }

        ~WeakRefController() {
            if (mRefController) {
                ReleaseWeakRef();
                mRefController = nullptr;
            }
        }

        WeakRefController& operator=(const WeakRefController& other) {
            if (mRefController == other.mRefController)
                return *this;

            WeakRefController tmp(other);
            std::swap(*this, tmp);

            return *this;
        }

        WeakRefController& operator=(WeakRefController&& other) noexcept {
            if (mRefController == other.mRefController)
                return *this;

            this->~WeakRefController();
            mRefController = other.mRefController;
            other.mRefController = nullptr;

            return *this;
        }

        WeakRefController& operator=(const SharedRefController& other) {
            this->~WeakRefController();

            if (other.mRefController) {
                mRefController = other.mRefController;
                AddWeakRef();
            }

            return *this;
        }
    };

}

#endif //BERSERK_SHAREDPOINTERINTERNAL_HPP
