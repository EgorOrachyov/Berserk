//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERELEMENT_H
#define BERSERK_RENDERELEMENT_H

#include <Engine/EngineResources.h>
#include <Rendering/RenderLayer.h>
#include <RHI/RHIResources.h>

namespace Berserk
{

    /** Data, needed to render single sub-mesh of the render object */
    struct RENDER_API RenderElement : public Allocatable
    {
    public:

        RenderElement(MeshHandle mesh,
                      MeshNode node,
                      MaterialHandle material,
                      const Mat4x4f& transform,
                      RHIGeometryBufferRef buffer)

            : mesh(std::move(mesh)),
              node(node),
              material(std::move(material)),
              transform(transform),
              buffer(std::move(buffer))
        {

        }

        /** Mesh object */
        MeshHandle mesh;

        /** Part of the mesh - single node to render */
        MeshNode node;

        /** Material for that mesh [could be empty]  */
        MaterialHandle material;

        /** Transformation matrix fo sum-mesh (could be identity)  */
        Mat4x4f transform;

        /** RHI buffer with rendering hardware render data */
        RHIGeometryBufferRef buffer;

    };

} // namespace Berserk

#endif //BERSERK_RENDERELEMENT_H