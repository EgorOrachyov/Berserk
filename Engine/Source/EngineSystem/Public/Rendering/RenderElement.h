//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_RENDERELEMENT_H
#define BERSERK_RENDERELEMENT_H

#include <Rendering/RenderSceneInfo.h>
#include <RHI/RHIResources.h>

namespace Berserk
{

    /** Data, needed to render single sub-mesh of the Renderable object*/
    struct RENDER_API RenderElement
    {
    public:

        RenderElement(MeshNode& node, const MaterialRef& material, const RHIGeometryBufferRef& buffer)
            : node(node),
              material(material),
              buffer(buffer)
        {

        }

        /** Actual mesh node to render*/
        MeshNode& node;

        /** Material for that mesh  [could be empty]  */
        MaterialRef material;

        /** RHI buffer with rendering hardware render data */
        RHIGeometryBufferRef buffer;

    };

} // namespace Berserk

#endif //BERSERK_RENDERELEMENT_H