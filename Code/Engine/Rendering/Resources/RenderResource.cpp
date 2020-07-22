/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/RenderResource.h>

namespace Berserk {
    namespace Rendering {

        TRef<const RenderResource> RenderResource::gResourceList;

        bool RenderResource::isResourceLinked() const {
            return mPrevLink.isNotNull() || mNextLink.isNotNull() || (gResourceList.getPtr() == this);
        }

        void RenderResource::linkResource() {
            if (isResourceLinked()) return;

            auto head = getResourcesList();

            mNextLink = head;
            mPrevLink = nullptr;

            if (head.isNotNull()) head->mPrevLink = this;

            gResourceList = this;
        }

        void RenderResource::unlinkResource() {
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

        TRef<const RenderResource> RenderResource::getResourcesList() {
            return gResourceList;
        }

    }
}