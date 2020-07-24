/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERRESOURCE_H
#define BERSERK_RENDERRESOURCE_H

#include <String/CString.h>
#include <TRef.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Rendering module resource
         * Base class for any resource, owned by render module.
         *
         * @note This resources could be accessed only on render thread
         *       (job thread, which process render module update).
         * @note Render module message queue must be used if you want
         *       to create, destroy, modify resources from other threads
         *
         * @note All resources forms linked list for easier resources profiling.
         * @note Resources has string markers for debugging.
         */
        class RenderResource {
        public:

            virtual ~RenderResource() = default;

            /** @return True if resource initialized and ready for usage */
            virtual bool isInitialized() const = 0;

            /** @return True if resource RHI state is initialized */
            virtual bool isInitializedRHI() const = 0;

            /** @return Resource name for debugging */
            virtual CString getFriendlyName() const { return ""; };

            /** @return True if resource linked to the resources list */
            bool isResourceLinked() const;

            /** @return Next resource in list after that (possibly null) */
            TRef<const RenderResource> getNextLink() const { return mNextLink; }

            /** @return Prev resource in list after that (possibly null) */
            TRef<const RenderResource> getPrevLink() const { return mPrevLink; }

            /** @return First resource link in the list */
            static TRef<const RenderResource> getResourcesList();

        protected:

            /** Link resource to the resources linked list */
            void linkResource();

            /** Unlink resource from the resources linked list */
            void unlinkResource();

        private:
            mutable TRef<const RenderResource> mNextLink;
            mutable TRef<const RenderResource> mPrevLink;

            /** Render resources list */
            static TRef<const RenderResource> gResourceList;

        };

    }
}

#endif //BERSERK_RENDERRESOURCE_H