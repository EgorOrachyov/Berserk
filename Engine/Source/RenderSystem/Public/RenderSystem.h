//
// Created by Egor Orachyov on 2019-08-15.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include <Rendering/IRenderSystem.h>
#include <RenderScene.h>

namespace Berserk
{

    class RENDER_API RenderSystem final : public IRenderSystem
    {
    public:

        RenderSystem(const RHIDriverRef& driver, IAllocator& allocator = Allocator::get());

        ~RenderSystem() override = default;

        TSharedPtr<IRenderScene> createScene() override;

        void addScene(const TSharedPtr<IRenderScene> &scene) override;

        void removeScene(const TSharedPtr<IRenderScene> &scene) override;

    public:

        /**
         * Internal method - render single scene all the cameras view
         * @param scene Scene data to actually render
         */
        void render(const RenderScene& scene);

        // todo: implement IRenderSystem interface
        // todo: add get/set methods for properties
        // todo: add RHI reload ?

        RHIDriverRef mDriver;

    };


} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H