//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_RENDERCOMPONENT_H
#define BERSERK_RENDERCOMPONENT_H

#include <Components/Component.h>
#include <Rendering/Renderable.h>
#include <Rendering/RenderSceneInfo.h>

namespace Berserk
{
    enum ERenderDirtyFlags
    {
        RDF_Transform = SHIFT(0),
        RDF_Renderable = SHIFT(1),
        RDF_Settings = SHIFT(2),
        RDF_Everything = SHIFT(3)
    };

    /** Base class for any kind of primitive / geometry, which could be rendered by render system */
    class ENGINE_API RenderComponent : public Component
    {
    public:

        REFLECTABLE_OBJECT(RenderComponent);

    public:

#if DEBUG
        RenderComponent() : Component() { }
#endif

        /** Set this render object cast shadows */
        void setCastShadows(bool flag)
        {
            markDirtyIfFieldChanged(mCastShadows, flag);
            mCastShadows = flag;
        }

        /** Apply culling for this object for optimisation */
        void setApplyCulling(bool flag)
        {
            markDirtyIfFieldChanged(mApplyCulling, flag);
            mApplyCulling = flag;
        }

        /** Set drw wireframe on top of this object [debug facility] */
        void setDrawWireframe(bool flag)
        {
            markDirtyIfFieldChanged(mDrawWireframe, flag);
            mDrawWireframe = flag;
        }

        /** Set draw only wireframe [debug facility] */
        void setDrawWireframeOnly(bool flag)
        {
            markDirtyIfFieldChanged(mDrawWireframeOnly, flag);
            mDrawWireframeOnly = flag;
        }

        /** Set draw bounding volume of this render object [debug facility] */
        void setDrawBoundingVolume(bool flag)
        {
            markDirtyIfFieldChanged(mDrawBoundingVolume, flag);
            mDrawBoundingVolume = flag;
        }

        /** Set this object active (affects on the scene) [cause main and render thread sync] */
        void setIsActive(bool active)
        {
            markDirtyIfFieldChanged(mIsActive, active);
            mIsActive = active;
        }

        /** Set world position [cause main and render thread sync] */
        void setWorldPosition(const Vec3f& position)
        {
            mWorldPosition = position;
            renderComponent_markDirty(ERenderDirtyFlags::RDF_Transform);
        }

        /** Set world rotation [cause main and render thread sync] */
        void setWorldRotation(const Quatf& rotation)
        {
            mWorldRotation = rotation;
            renderComponent_markDirty(ERenderDirtyFlags::RDF_Transform);
        }

        /** @return True, if this render object can cast shadows */
        bool castShadows() const { return mCastShadows; }

        /** @return True, if can use frustum (or something else) culling for this object */
        bool applyCulling() const { return mApplyCulling; }

        /** @return True, if need draw wireframe for debug purpose on top of the object */
        bool drawWireframe() const { return mDrawWireframe; }

        /** @return True, if need draw wire frame only (without filled polygons) */
        bool drawWireframeOnly() const { return mDrawWireframeOnly; }

        /** @return True, if need draw all the bounding boxes of the object */
        bool drawBoundingVolume() const { return mDrawBoundingVolume; }

        /** @return True if this object is active/renderable on the scene */
        bool isActive() const { return mIsActive; }

        /** @return World space position */
        const Vec3f& getWorldPosition() const { return mWorldPosition; }

        /** @return World space rotation */
        const Quatf& getWorldRotation() const { return mWorldRotation; }

        /** @return Render data of this object [must stay immutable] */
        virtual const TSharedPtr<Renderable> &getRenderable() const = 0;

        /** @return Scene info of this object [for render thread] */
        const TSharedPtr<class RenderableSceneInfo> &getSceneInfo() const { return mSceneInfo; }

        /** @return Creates render scene info of this object [for render thread] */
        const TSharedPtr<class RenderableSceneInfo> &createSceneInfo();

    protected:

        /** Mark dirty if bool field was changed */
        void markDirtyIfFieldChanged(bool oldValue, bool newValue)
        {
            if (oldValue != newValue) renderComponent_markDirty(ERenderDirtyFlags::RDF_Settings);
        }

        /** Dirty flags to sync with render system */
        virtual void renderComponent_markDirty(uint32 flags = ERenderDirtyFlags::RDF_Everything) {}

    private:

        // todo: add lod mesh array for optimised distance view
        // todo: add shadow mesh

        /** If this render object can cast shadows */
        bool mCastShadows;

        /** Can use frustum (or something else) culling for this object */
        bool mApplyCulling;

        /** Draw wireframe for debug purpose on top of the object */
        bool mDrawWireframe;

        /** Draw wire frame only (without filled polygons) */
        bool mDrawWireframeOnly;

        /** Draw all the bounding boxes of the object */
        bool mDrawBoundingVolume;

        /** Whether this object is active in the scene */
        bool mIsActive;

        /** World space position */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** World space rotation */
        Quatf mWorldRotation = Quatf();

        /** Scene info for renderer thread of this render object */
        TSharedPtr<class RenderableSceneInfo> mSceneInfo;

    };

} // namespace Berserk

#endif //BERSERK_RENDERCOMPONENT_H