/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDEROBJECT_H
#define BERSERK_RENDEROBJECT_H

#include <TEnumMask.h>
#include <String/CString.h>
#include <Math/Transformf.h>
#include <Containers/TArrayView.h>
#include <Scene/RenderObjectBounds.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Render object
         *
         * Base class for any object, which can be rendered on the scene.
         * Generally, provides a set of events functions, and general rendering settings.
         * Intended to be subclassed in order to add new render object types.
         */
        class RenderObject {
        public:
            RenderObject();
            virtual ~RenderObject() = default;

            /** Called after occlusion query test on provided by this object items */
            virtual void onOcclusionQueryResults(const class SceneView &sceneView, const TArrayView<bool> &results) = 0;

            /** Called to update dirty state */
            virtual void onUpdateDirty() = 0;

            /** @return Optional debug name */
            const CString& getName() const { return mName; }

            /** @return True if can cast shadows  */
            bool castShadows() const { return mCastShadows; }

            /** @return True if currently visible */
            bool isVisible() const { return mVisible; }

            /** @return True if has movement velocity */
            bool hasVelocity() const { return mHasVelocity; }

            /** @return True if can apply culling for this object */
            bool canBeCulled() const { return mCanBeCulled; }

            /** @return True if this object is already added to the scene */
            bool isAttachedToScene() const { return mScene == nullptr; }

            /** @return True if this object is added to the specified scene */
            bool isAttachedToScene(const class Scene& scene) const { return mScene == &scene; }

            /** @return Min culling distance from camera */
            float getMinViewDistance() const { return mMinViewDistance; }

            /** @return Max culling distance from camera */
            float getMaxViewDistance() const { return mMaxViewDistance; }

            /** @return Index in the scene primitives */
            uint32 getSceneIndex() const { return mSceneIndex; }

            /** Set object world space transform  */
            void setTransform(const Transformf& t);

            /** Get object bounds */
            void getBounds(RenderObjectBounds& bounds);

            /** @return World-space object transform */
            const Transformf& getTransform() const { return mTransform; }

            /** @return Local to world cached matrix */
            const Mat4x4f &getCachedLocalToWorld() const { return mCachedLocalToWorld; }

            /** @return World space object bounds */
            const Aabbf &getCachedWorldBounds() const { return mCachedWorldBounds; }

            /** @return Local space bounds */
            const Aabbf& getBounds() const { return mBounds; }

        protected:

            friend class Scene;

            /** Called once when the object added to the scene */
            void attachToScene(class Scene &scene, uint32 index) { mScene = &scene;  mSceneIndex = index; }

            /** Called once when the object removed from the scene */
            void detachFromScene() { mScene = nullptr; mSceneIndex = -1; }

            /** Updates primitive index in the scene */
            void updateSceneIndex(uint32 index) { mSceneIndex = index; }

        protected:

            /** Optional debug name */
            CString mName;

            /** True if can cast shadows  */
            bool mCastShadows : 1;

            /** True if currently visible */
            bool mVisible : 1;

            /** True if has movement velocity */
            bool mHasVelocity : 1;

            /** True if can apply culling for this object */
            bool mCanBeCulled : 1;

            /** Min culling distance from camera */
            float mMinViewDistance = 0.0f;

            /** Max culling distance from camera */
            float mMaxViewDistance = 10000.0f;

            /** World-space object transform */
            Transformf mTransform;

            /** Local to world cached matrix */
            Mat4x4f mCachedLocalToWorld;

            /** World space object bounds */
            Aabbf mCachedWorldBounds;

            /** Local space objects bounds */
            Aabbf mBounds;

            /** Scene this object belongs to */
            class Scene* mScene = nullptr;

            /** Index in the scene primitives */
            uint32 mSceneIndex = -1;
        };

    }
}

#endif //BERSERK_RENDEROBJECT_H