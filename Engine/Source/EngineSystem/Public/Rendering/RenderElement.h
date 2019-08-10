//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERELEMENT_H
#define BERSERK_RENDERELEMENT_H

#include <Engine/EngineResources.h>
#include <RHI/RHIResources.h>

namespace Berserk
{

    /** Data, needed to render single sub-mesh of the render object */
    struct RENDER_API RenderElement : public Allocatable
    {
    public:

        RenderElement(const MeshHandle& mesh,
                      const MeshNode& node,
                      const MaterialHandle& material,
                      const RHIGeometryBufferRef& buffer)
            : mesh(mesh),
              node(node),
              material(material),
              buffer(buffer)
        {

        }

        /** Mesh object */
        MeshHandle mesh;

        /** Part of the mesh - single node to render */
        MeshNode node;

        /** Material for that mesh [could be empty]  */
        MaterialHandle material;

        /** RHI buffer with rendering hardware render data */
        RHIGeometryBufferRef buffer;

    };

} // namespace Berserk

#endif //BERSERK_RENDERELEMENT_H