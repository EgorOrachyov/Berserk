//
// Created by Egor Orachyov on 2019-08-22.
//

#ifndef BERSERK_PIPELINEPASSCONTEXT_H
#define BERSERK_PIPELINEPASSCONTEXT_H

#include <RenderScene.h>

namespace Berserk
{

    /**
     * Info passed through pipeline stages by pipeline state scheduler.
     * This info generated each time by render system before each single
     * camera to window scene rendering.
     *
     * @note If N scenes each one contains M cameras, then this context info
     *       will be generated N * M times, for each scene and camera.
     *
     * @note Contains all the info, needed for pipeline stages in time of the execution
     */
    class RENDER_API PipelinePassContext
    {
    public:

        /** Driver for render hardware state access */
        RHIDriver* driver;

        /** Scene, contains info about currently rendered objects */
        RenderScene* scene;

        /** Currently rendered camera info */
        CameraSceneInfo* camera;

        /** Window, to draw result */
        IWindow* window;

        // todo: add render queue access
        // todo: add rendering setting reference
        // todo: add info for profiling

    };

} // namespace Berserk

#endif //BERSERK_PIPELINEPASSCONTEXT_H