//
// Created by Egor Orachyov on 08.08.2018.
//

#include "Render/GLRenderNode.h"
#include "Render/RenderSystem.h"
#include "Math/UtilityMatrices.h"

namespace Berserk
{

    GLRenderNode::GLRenderNode()
    {
        mMaxViewDistance = 0;
        mType = RenderNodeType::RNT_NOT_RENDER_NODE;
        mMaterial = nullptr;
        mRenderMesh = nullptr;
        mShadowMesh = nullptr;
        mPosition = Vector3f(0.0);
        mTransformation = newMatrix(1.0);
    }

    void GLRenderNode::destroy()
    {
        if (mMaterial) gRenderSystem->getMaterialManagerPtr()->deleteMaterial(mMaterial);
        if (mRenderMesh) gRenderSystem->getRenderMeshManagerPtr()->deleteRenderMesh(mRenderMesh);
        if (mShadowMesh) gRenderSystem->getRenderMeshManagerPtr()->deleteRenderMesh(mShadowMesh);
    }

    void GLRenderNode::setMaxViewDistance(FLOAT32 distance)
    {
        mMaxViewDistance = distance;
    }

    FLOAT32 GLRenderNode::getMaxViewDistance() const
    {
        return mMaxViewDistance;
    }

    void GLRenderNode::setRenderNodeType(UINT32 type)
    {
        mType = type;
    }

    UINT32 GLRenderNode::getRenderNodeType() const
    {
        return mType;
    }

    void GLRenderNode::setPosition(const Vector3f& position)
    {
        mPosition = position;
    }

    const Vector3f & GLRenderNode::getPosition() const
    {
        return mPosition;
    }

    void GLRenderNode::setTransformation(const Matrix4x4f &transformation)
    {
        mTransformation = transformation;
    }

    const Matrix4x4f& GLRenderNode::getTransformation() const
    {
        return mTransformation;
    }

    void GLRenderNode::setRenderMesh(RenderMesh* mesh)
    {
        ASSERT(mesh, "GLRenderNode: An attempt to pass NULL RenderMesh");
        mRenderMesh = dynamic_cast<GLRenderMesh*>(mesh);
    }

    RenderMesh* GLRenderNode::getRenderMesh()
    {
        return mRenderMesh;
    }

    void GLRenderNode::setShadowMesh(RenderMesh* mesh)
    {
        ASSERT(mesh, "GLRenderNode: An attempt to pass NULL ShadowMesh");
        mShadowMesh = dynamic_cast<GLRenderMesh*>(mesh);
    }

    RenderMesh* GLRenderNode::getShadowMesh()
    {
        return mShadowMesh;
    }

    void GLRenderNode::setMaterial(Material* material)
    {
        ASSERT(material, "GLRenderNode: An attempt to pass NULL Material");
        mMaterial = dynamic_cast<GLMaterial*>(material);
    }

    Material* GLRenderNode::getMaterial()
    {
        return mMaterial;
    }

    bool GLRenderNode::isLoaded() const
    {
        return (mMaterial != nullptr && mRenderMesh != nullptr && mShadowMesh != nullptr);
    }

    const bool GLRenderNode::operator==(const GLRenderNode &node) const
    {
        return (this == &node);
    }

} // namespace Berserk