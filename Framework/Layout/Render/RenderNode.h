//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_RENDERNODE_H
#define BERSERKENGINE_RENDERNODE_H

#include "Math/Matrix4x4f.h"
#include "Material.h"

namespace Berserk
{
    enum RenderNodeType
    {
        RNT_NOT_RENDER_NODE,
        RNT_LIGHT_NODE,
        RNT_OBJECT,
        RNT_CHARACTER_NODE
    };

    class RenderNode
    {
    public:

        virtual ~RenderNode() = default;

        virtual void setMaxViewDistance(FLOAT32 distance) = 0;
        virtual FLOAT32 getMaxViewDistance() const = 0;

        virtual void setRenderNodeType(RenderNodeType type) = 0;
        virtual RenderNodeType getRenderNodeType() const = 0;

        virtual void setPosition(const Vector3f& position) = 0;
        virtual Vector3f getPosition() const = 0;

        virtual void setTransormation(const Matrix4x4f& transformation) = 0;
        virtual const Matrix4x4f& getTransformation() const = 0;

        virtual void setMaterial(Material* material) = 0;
        virtual Material* getMaterial() = 0;

        // todo: add geom mesh

        virtual void setVisible(bool setIn) = 0;

        virtual bool isLoaded() const = 0;
        virtual bool isVisible() const = 0;
    };
}

#endif //BERSERKENGINE_RENDERNODE_H