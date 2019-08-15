//
// Created by Egor Orachyov on 2019-08-15.
//

#ifndef BERSERK_RENDERUTILS_H
#define BERSERK_RENDERUTILS_H

#include <RenderSceneInfo.h>
#include <RHI/RHIDriver.h>

namespace Berserk
{

    /**
     * Utils used among render system for creating,
     * updating objects and render resources
     */
    class RENDER_API RenderUtils
    {
    public:

        /**
         * Creates geometry buffer for renderable scene info data
         * @param driver RHI driver to create vertex, index, geometry buffers
         * @param[in,out] data Actual data about render object. Here will be stored allocated geometry buffer
         */
        static void createGeometryBuffer(RHIDriverRef& driver, RenderableSceneInfoRef& data);

    };

} // namespace Berserk

#endif //BERSERK_RENDERUTILS_H