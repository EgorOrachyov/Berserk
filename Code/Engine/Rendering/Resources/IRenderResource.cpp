/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/IRenderResource.h>

namespace Berserk {
    namespace Rendering {

        TRef<const IRenderResource> IRenderResource::gResourceList;

        bool IRenderResource::isResourceLinked() const {
            return mPrevLink.isNotNull() || mNextLink.isNotNull() || (gResourceList.getPtr() == this);
        }

        void IRenderResource::linkResource() {
            if (isResourceLinked()) return;

            auto head = getResourcesList();

            mNextLink = head;
            mPrevLink = nullptr;

            if (head.isNotNull()) head->mPrevLink = this;

            gResourceList = this;
        }

        void IRenderResource::unlinkResource() {
            if (!isResourceLinked()) return;

            auto prev = mPrevLink;
            auto next = mNextLink;

            if (prev.isNotNull()) prev->mNextLink = next;
            if (next.isNotNull()) next->mPrevLink = prev;

            auto head = getResourcesList();
            if (head == this) gResourceList = next;

            mPrevLink = nullptr;
            mNextLink = nullptr;
        }

        TRef<const IRenderResource> IRenderResource::getResourcesList() {
            return gResourceList;
        }

    }
}