//
// Created by Egor Orachyov on 02.03.2019.
//

#ifndef BERSERK_IMATERIAL_H
#define BERSERK_IMATERIAL_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Resource/IResource.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/ITexture.h"

namespace Berserk::Resources
{

    /**
     * Material Interface class which defines material and object properties,
     * its appearance in 3D scene in time of rendering. Allows to configure
     * shader technique, active maps, interaction with lights and shadows.
     */
    class IMaterial : public IResource
    {
    public:

        /**
         * MaterialType defines appearance of the material, applied rendering
         * technique, produce or note shadows, require additional scene information,
         * such as local reflection map, etc.
         *
         * Note: rendering engine checks material type / properties from 0 to N..,
         * therefore, some properties can disable others (with lower priority)
         */
        enum MaterialType
        {
            eMT_WIRE_FRAME                  = SHIFT(1),     //! Render as wire frame (no polygon fill)
            eMT_CAST_SHADOWS                = SHIFT(1),     //! If can cast shadows (can overlap the light)
            eMT_NO_LIGHTNING                = SHIFT(2),     //! Raw coloring without light computation at all
            eMT_BLINN_PHONG                 = SHIFT(3),     //! Blinn-Phong lightning equation
            eMT_PHYSICALLY_BASED            = SHIFT(4),     //! Physically Based rendering equation
            eMT_EMISSIVE                    = SHIFT(5),     //! Emmit light as a light source (used for bloom effect)
            eMT_ALPHA_BLEND                 = SHIFT(6),     //! Has active alpha component (requires alpha pass)
            eMT_UI_MATERIAL                 = SHIFT(7),     //! For rendering UI elements
            eMT_REQUIRE_NEAREST_CUBE_MAP    = SHIFT(8),     //! Need local reflection map to create reflections
            eMT_RESERVED_0                  = SHIFT(9),     //! Reserved!
            eMT_RESERVED_1                  = SHIFT(10),    //! Reserved!
            eMT_RESERVED_2                  = SHIFT(11),    //! Reserved!
            eMT_RESERVED_3                  = SHIFT(12),    //! Reserved!
            eMT_RESERVED_4                  = SHIFT(13),    //! Reserved!
            eMT_RESERVED_5                  = SHIFT(14),    //! Reserved!
            eMT_RESERVED_6                  = SHIFT(15),    //! Reserved!
            eMT_RESERVED_7                  = SHIFT(16),    //! Has the lowest priority
        };

        /**
         * MaterialLayer defines available for the material texture maps,
         * which will be used by rendering techniques in time of light computation.
         */
        enum MaterialLayer
        {
            eML_ALBEDO_MAP           = SHIFT(1),    //! Texture with default color
            eML_NORMAL_MAP           = SHIFT(2),    //! Surface relief info (light interaction)
            eML_METALLIC_MAP         = SHIFT(3),    //! Metallic surface properties
            eML_ROUGHNESS_MAP        = SHIFT(4),    //! Roughness surface properties
            eML_AMBIENT_MAP          = SHIFT(5),    //! Ambient occlusion map for material (always optional)
            eML_DISPLACEMENT_MAP     = SHIFT(6),    //! Surface relief info (texture sampling)
            eML_RESERVED_0           = SHIFT(7),    //! Reserved!
            eML_RESERVED_1           = SHIFT(8),    //! Reserved!
            eML_RESERVED_2           = SHIFT(9),    //! Reserved!
            eML_RESERVED_3           = SHIFT(10),   //! Reserved!
            eML_RESERVED_4           = SHIFT(11),   //! Reserved!
            eML_RESERVED_5           = SHIFT(12),   //! Reserved!
            eML_RESERVED_6           = SHIFT(13),   //! Reserved!
            eML_RESERVED_7           = SHIFT(14),   //! Has the lowest priority
        };

    public:

        /**
         * Set material type via bit mask of enums
         * @param type Bit mask with material type
         */
        virtual void setMaterialType(uint32 type) = 0;

        /**
         * Enable or disable chosen material layer
         * @warning If the layer is not attached but marked as active,
         *          method will generate warning message

         * @param layer  Layer id
         * @param active Flag to set active (or disable)
         */
        virtual void setMaterialLayer(MaterialLayer layer, bool active) = 0;

        /**
         * Set active material layers via bit mask
         * @warning If the layer is not attached but marked as active,
         *          method will generate warning message
         *
         * @param Active layers bit mask
         */
        virtual void setMaterialLayers(uint32 layersMask) = 0;

        /** @return Material type represented as a bit mask */
        virtual uint32 getMaterialType() = 0;

        /** @return Material active layers as a bit mask */
        virtual uint32 getMaterialLayers() = 0;

        /** @return Number of attached layers to this material */
        virtual uint32 getNumAttachedLayers() = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

        /** @return Material main color in RGBA format  */
        virtual const Vec4f& getColorComponet() const = 0;

        /** @return Pointer to attached layer via id or nullptr */
        virtual const ITexture* getMaterialLayer(MaterialLayer layer) const = 0;

    };

} // namespace Berserk::Render

#endif //BERSERK_IMATERIAL_H