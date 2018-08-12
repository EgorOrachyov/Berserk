//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_GLRENDERNODE_H
#define BERSERKENGINE_GLRENDERNODE_H

#include "Render/RenderNode.h"
#include "Render/GLMaterial.h"
#include "Render/GLRenderMesh.h"

namespace Berserk
{

    class GLRenderNode : public RenderNode
    {
    public:

        GLRenderNode();
        virtual ~GLRenderNode() = default;

        void destroy() override;

        void setMaxViewDistance(FLOAT32 distance) override;
        FLOAT32 getMaxViewDistance() const override;

        void setRenderNodeType(UINT32 type) override;
        UINT32 getRenderNodeType() const override;

        void setPosition(const Vector3f& position) override;
        const Vector3f & getPosition() const override;

        void setTransformation(const Matrix4x4f &transformation) override;
        const Matrix4x4f& getTransformation() const override;

        void setRenderMesh(RenderMesh* mesh) override;
        RenderMesh* getRenderMesh() override;

        void setShadowMesh(RenderMesh* mesh) override;
        RenderMesh* getShadowMesh() override;

        void setMaterial(Material* material) override;
        Material* getMaterial() override;

        bool isLoaded() const override;

        const bool operator == (const GLRenderNode& node) const;

    private:

        FLOAT32 mMaxViewDistance;
        UINT32  mType;

        INT8 mIsVisible;
        INT8 data1;
        INT8 data2;
        INT8 data3;

        GLMaterial*   mMaterial;
        GLRenderMesh* mRenderMesh;
        GLRenderMesh* mShadowMesh;

        Vector3f   mPosition;
        Matrix4x4f mTransformation;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLRENDERNODE_H