//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include "Containers/List.h"
#include "Containers/LinkedList.h"
#include "System/System.h"

#include "GPUBuffer.h"
#include "GBuffer.h"
#include "DepthMap.h"
#include "CubeDepthMap.h"
#include "RenderNode.h"

#include "Objects/Cameras/Camera.h"
#include "Objects/Lights/SpotLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/DirectionalLight.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "RenderMeshManager.h"
#include "TextureImporter.h"

namespace Berserk
{
    enum LightInfo
    {
        LI_MAX_SPOT_LIGHTS          = 16,
        LI_MAX_POINT_LIGHTS         = 16,
        LI_MAX_DIRECTIONAL_LIGHTS   = 16
    };

    enum ShadowInfo
    {
        SI_QUALITY_LOW      = 1,
        SI_QUALITY_MEDIUM   = 2,
        SI_QUALITY_HIGH     = 3,

        SI_MAX_DIRECTIONAL_SHADOWS      = 4,
        SI_MAX_OMNIDIRECTIONAL_SHADOWS  = 2,

        SI_MAX_DIR_SHADOW_SOURCES       = 2,
        SI_MAX_SPOT_SHADOW_SOURCES      = 2,
        SI_MAX_POINT_SHADOW_SOURCES     = 2,

        SI_MAP_SIZE_QUALITY_LOW         = 512,
        SI_MAP_SIZE_QUALITY_MEDIUM      = 1024,
        SI_MAP_SIZE_QUALITY_HIGH        = 2048,

        SI_DIR_MAP_SLOT0        = 10,
        SI_SPOT_MAP_SLOT0       = 12,
        SI_POINT_MAP_SLOT0      = 14,
    };

    class RenderSystem : public System
    {
    public:

        virtual ~RenderSystem() = default;

        virtual void renderPass1() = 0;
        virtual void renderPass2() = 0;

        virtual const CString& getRenderName() const = 0;
        virtual const CString& getShadingLanguageName() const = 0;

        virtual void setRenderCamera(Camera *camera) = 0;
        virtual void setAmbientLight(const Vector3f& light) = 0;
        virtual void setClearColor(const Vector4f& color) = 0;
        virtual void setShadowQuality(ShadowInfo quality) = 0;
        virtual void setWindowName(const CStaticString& name) = 0;

        virtual Camera* getRenderCamera() = 0;
        virtual const Vector3f& getAmbientLightSource() const = 0;
        virtual const Vector4f& getClearColor() const = 0;
        virtual ShadowInfo getShadowQuality() const = 0;
        virtual UINT32 getShadowMapSize() const = 0;
        virtual const CStaticString& getWindowName() const = 0;

        virtual void setExposure(FLOAT32 exposure) = 0;
        virtual void setLuminanceThresh(FLOAT32 luminance) = 0;
        virtual void setGammaCorrection(FLOAT32 gamma) = 0;

        virtual FLOAT32 getExposure() const = 0;
        virtual FLOAT32 getLuminanceThresh() const = 0;
        virtual FLOAT32 getGammaCorrection() const = 0;

        virtual UINT32 getWindowWidth() const = 0;
        virtual UINT32 getWindowHeight() const = 0;
        virtual void   getWindowSize(UINT32 &width, UINT32 &height) const = 0;

        virtual UINT32 getPixelWindowWidth() const = 0;
        virtual UINT32 getPixelWindowHeight() const = 0;
        virtual void   getPixelWindowSize(UINT32& width, UINT32& height) const = 0;

        virtual UINT32 getOldPixelWindowWidth() const = 0;
        virtual UINT32 getOldPixelWindowHeight() const = 0;
        virtual void   getOldPixelWindowSize(UINT32& width, UINT32& height) const = 0;

        virtual UINT32 getWindowPosX() const = 0;
        virtual UINT32 getWindowPosY() const = 0;
        virtual void   getWindowPos(UINT32& posX, UINT32& posY) const = 0;

        virtual bool isReSized() = 0;

        virtual void queueShadowLightSource(SpotLight* light)= 0;
        virtual void queueShadowLightSource(PointLight* light)= 0;
        virtual void queueShadowLightSource(DirectionalLight* light)= 0;
        virtual void queueLightSource(SpotLight* light) = 0;
        virtual void queueLightSource(PointLight* light) = 0;
        virtual void queueLightSource(DirectionalLight* light) = 0;
        virtual void queueRenderNode(RenderNode* node) = 0;

        virtual List<SpotLight*>        &getSpotShadowSources()= 0;
        virtual List<PointLight*>       &getPointShadowSources()= 0;
        virtual List<DirectionalLight*> &getDirectionalShadowSources()= 0;
        virtual List<SpotLight*>        &getSpotLightSources() = 0;
        virtual List<PointLight*>       &getPointLightSources() = 0;
        virtual List<DirectionalLight*> &getDirectionalLightSources() = 0;
        virtual List<RenderNode*>       &getRenderNodeSources() = 0;

        virtual GBuffer* getGBuffer() = 0;
        virtual DepthMap* getDirDepthMaps() = 0;
        virtual DepthMap* getSpotDepthMaps() = 0;
        virtual CubeDepthMap* getPointDepthMaps() = 0;

        virtual TextureManager &getTextureManagerRef() = 0;
        virtual TextureManager *getTextureManagerPtr() = 0;

        virtual MaterialManager &getMaterialManagerRef() = 0;
        virtual MaterialManager *getMaterialManagerPtr() = 0;

        virtual RenderMeshManager &getRenderMeshManagerRef() = 0;
        virtual RenderMeshManager *getRenderMeshManagerPtr() = 0;

        // todo: add light create functions

        virtual RenderNode* createRenderNode() = 0;
        virtual void deleteRenderNode(RenderNode* node) = 0;

        static RenderSystem &getRenderSystemRef();
        static RenderSystem *getRenderSystemPtr();

    };

    /// Should be initialized via Application Context
    extern RenderSystem *gRenderSystem;

} // namespace Berserk

#endif //BERSERK_RENDERSYSTEM_H
