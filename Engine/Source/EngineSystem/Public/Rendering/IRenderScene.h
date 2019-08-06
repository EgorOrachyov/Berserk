//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_IRENDERSCENE_H
#define BERSERK_IRENDERSCENE_H

#include <Components/CameraComponent.h>
#include <Components/SpotLightComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/DirectionalLightComponent.h>

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
        ENGINE_API virtual void addLight(SpotLightComponent& light) = 0;

        /**
         * Adds light to the render scene. If light is active, it will be
         * immediately visible in rendered scene.
         * @param light - Light component to add
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void addLight(PointLightComponent& light) = 0;

        /**
         * Adds light to the render scene. If light is active, it will be
         * immediately visible in rendered scene.
         * @param light - Light component to add
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void addLight(DirectionalLightComponent& light) = 0;

        /**
         * Removes light from the render scene. After that light has no effect on
         * the rendered scene and does not handled by render thread.
         * @param light - Light component to remove
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void removeLight(SpotLightComponent& light) = 0;

        /**
         * Removes light from the render scene. After that light has no effect on
         * the rendered scene and does not handled by render thread.
         * @param light - Light component to remove
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void removeLight(PointLightComponent& light) = 0;

        /**
         * Removes light from the render scene. After that light has no effect on
         * the rendered scene and does not handled by render thread.
         * @param light - Light component to remove
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void removeLight(DirectionalLightComponent& light) = 0;

        /**
         * Updates light sources params (transformation/bounds/properties)
         * @param light - Light component to update
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void updateLight(SpotLightComponent& light) = 0;

        /**
         * Updates light sources params (transformation/bounds/properties)
         * @param light - Light component to update
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void updateLight(PointLightComponent& light) = 0;

        /**
         * Updates light sources params (transformation/bounds/properties)
         * @param light - Light component to update
         * @note Light reference must be valid till this the function is executed
         */
        ENGINE_API virtual void updateLight(DirectionalLightComponent& light) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRENDERSCENE_H