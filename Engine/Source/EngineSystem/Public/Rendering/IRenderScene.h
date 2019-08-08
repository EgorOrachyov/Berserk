//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_IRENDERSCENE_H
#define BERSERK_IRENDERSCENE_H

#include <Components/CameraComponent.h>
#include <Components/LightComponent.h>
#include <Components/RenderComponent.h>

namespace Berserk
{

    /**
     * Interface to private render engine scene implementation.
     * Base class for communication between main and render thread.
     * Provides access to the rendered engine scene.
     *
     * @note Single-threaded
     */
    class ENGINE_API IRenderScene
    {
    public:

        /**
         * Adds light to the render scene. If light is active, it will be
         * immediately visible in rendered scene.
         * @param light - Light component to add
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void addLight(LightComponent& light) = 0;

        /**
         * Removes light from the render scene. After that light has no effect on
         * the rendered scene and does not handled by render thread.
         * @param light - Light component to remove
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void removeLight(LightComponent& light) = 0;

        /**
         * Updates light sources params (transformation/bounds/properties)
         * @param light - Light component to update
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void updateLight(LightComponent& light) = 0;

        /**
         * Adds render object to the render scene. If object is active, it will be
         * immediately visible in rendered scene.
         * @param object - Render object component to add
         * @note Object reference must be valid till this the function is executed
         */
        ENGINE_API virtual void addRenderable(RenderComponent& object) = 0;

        /**
         * Removes render object from the render scene. After that object won't occur
         * on the scene and won't be handled by render thread.
         * @param object - Render object component to add
         * @note Object reference must be valid till this the function is executed
         */
        ENGINE_API virtual void removeRenderable(RenderComponent& object) = 0;

        /**
         * Updates render object params (transformation/bounds/properties/renderable data)
         * @param object - Render object component to add
         * @note Object reference must be valid till this the function is executed
         */
        ENGINE_API virtual void updateRenderable(RenderComponent& object) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERSCENE_H