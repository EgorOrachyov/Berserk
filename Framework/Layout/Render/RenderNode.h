//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_RENDERNODE_H
#define BERSERKENGINE_RENDERNODE_H

#include "Math/Matrix4x4f.h"
#include "Material.h"
#include "RenderMesh.h"

namespace Berserk
{
    enum RenderNodeType
    {
        RNT_NOT_RENDER_NODE,
        RNT_LIGHT_NODE,
        RNT_OBJECT,
        RNT_OBJECT_NO_SAHDOWS,
        RNT_CHARACTER_NODE
    };

    class RenderNode
    {
    public:

        virtual ~RenderNode() = default;
        virtual void destroy() = 0;

        virtual void setMaxViewDistance(FLOAT32 distance) = 0;
        virtual FLOAT32 getMaxViewDistance() const = 0;

        virtual void setRenderNodeType(UINT32 type) = 0;
        virtual UINT32 getRenderNodeType() const = 0;

        virtual void setPosition(const Vector3f& position) = 0;
        virtual const Vector3f & getPosition() const = 0;

        virtual void setTransformation(const Matrix4x4f &transformation) = 0;
        virtual const Matrix4x4f& getTransformation() const = 0;

        virtual void setRenderMesh(RenderMesh* mesh) = 0;
        virtual RenderMesh* getRenderMesh() = 0;

        virtual void setShadowMesh(RenderMesh* mesh) = 0;
        virtual RenderMesh* getShadowMesh() = 0;

        virtual void setMaterial(Material* material) = 0;
        virtual Material* getMaterial() = 0;

        virtual bool isLoaded() const = 0;
    };
}

#endif //BERSERKENGINE_RENDERNODE_H