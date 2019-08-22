//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include <Resource/Resource.h>
#include <RHI/RHIResources.h>
#include <Math/Vec4f.h>

namespace Berserk
{

    /** Default engine techniques for rendering basic materials */
    enum EMaterialTechnique
    {
        /** Raw coloring without light computation at all */
        MT_NoLightning,

        /** Render as wire frame (no polygon fill) */
        MT_Wireframe,

        /** Blinn-Phong lightning equation */
        MT_Phong,

        /** Physically Based rendering equation */
        MT_PBR
    };

    /**
     * Base material class. Defines objects appearance and
     * rendering properties on the scene. Stores all the color and
     * texture data, needed to render single objects.
     *
     * @note Contains loaded textures references, colors,
     *       rendering modes, and shading techniques.
     *
     * @note Base material class appears only as container for the
     *       data and provides default render logic.
     *
     * @note todo: All the user-defined logic will be added later via render technique and pass classes.
     *
     * @note todo: Add custom user defined shader compilation and support
     */
    class ENGINE_API Material : public Resource
    {
    public:

        ~Material() override = default;

        /**
         * Set material asset predefined id for sort on render queue
         * @param id ID to set
         */
        void setMaterialID(uint32 id) { mMaterialID = id; }

        /**
         * Sets material technique enum
         * @param technique To set
         */
        void setTechnique(EMaterialTechnique technique) { mTechnique = technique; }

        /**
         * Set this material as with default technique
         * @param use True to use default
         */
        void setDefaultTechnique(bool use) { mIsDefault = use; }

        /**
         * Set this material as transparent
         * @param transparent True draw with alpha blending
         */
        void setTransparent(bool transparent) { mIsTransparent = transparent; }

        /**
         * Set color value [debug]
         * @note Material must not be modified after creation
         */
        void setColor(const Vec4f& color) { mColor = color; }

        /**
         * Set color value [debug]
         * @note Material must not be modified after creation
         */
        void setEmissiveColor(const Vec4f& color) { mEmissiveColor = color; }

        /**
         * Set color value [debug]
         * @note Material must not be modified after creation
         */
        void setWireframeColor(const Vec4f& color) { mWireframeColor = color; }

        /** @return Material id for render queue sort sort operation */
        uint32 getMaterialID() const { return mMaterialID; }

        /** @return Predefined default material technique */
        EMaterialTechnique getTechnique() const { return mTechnique; }

        /** @return True, whether this material uses default technique for rendering */
        bool useDefaultTechnique() const { return mIsDefault; }

        /** @return True, if this material requires alpha blending */
        bool isTransparent() const { return mIsTransparent; }

        /** @return Color value */
        const Vec4f& getColor() const { return mColor; }

        /** @return Color value */
        const Vec4f& getEmissiveColor() const { return mEmissiveColor; }

        /** @return Color value */
        const Vec4f& getWireframeColor() const { return mWireframeColor; }

        /**
         * Allows to set all the uniform variables for default material pass
         * @note This is only for default material techniques
         *
         * @param program Shader program for rendering (provided by render system)
         */
        virtual void uniform(RHIShaderProgramRef &program) { };

    protected:

        /** ID used for sorting on render queue */
        uint32 mMaterialID = 0;

        /** One of predefined technique to use */
        EMaterialTechnique mTechnique = MT_Wireframe;

        /** Shows, whether this material uses default render technique */
        bool mIsDefault = true;

        /** Whether this material requires alpha blending */
        bool mIsTransparent = false;

        /** Default material color */
        Vec4f mColor = Vec4f(1);

        /** Default material emissive color */
        Vec4f mEmissiveColor = Vec4f(0);

        /** Default material wireframe color */
        Vec4f mWireframeColor = Vec4f(1);

    };

} // namespace Berserk

#endif //BERSERK_MATERIAL_H