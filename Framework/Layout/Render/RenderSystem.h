//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERK_RENDERSYSTEM_H
#define BERSERK_RENDERSYSTEM_H

#include "Containers/List.h"
#include "Containers/LinkedList.h"
#include "System/System.h"

#include "GPUBuffer.h"
#include "RenderNode.h"
#include "Objects/Cameras/Camera.h"
#include "Objects/Lights/SpotLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/DirectionalLight.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "RenderMeshManager.h"

namespace Berserk
{
    enum LightInfo
    {
        LI_MAX_SPOT_LIGHTS          = 16,
        LI_MAX_POINT_LIGHTS         = 16,
        LI_MAX_DIRECTIONAL_LIGHTS   = 16
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
        virtual Camera* getRenderCamera() = 0;

        virtual void setAmbientLight(const Vector3f& light) = 0;
        virtual const Vector3f& getAmbientLightSource() = 0;

        virtual void setClearColor(const Vector4f& color) = 0;
        virtual const Vector4f& getClearColor() = 0;

        virtual void setExposure(FLOAT32 exposure) = 0;
        virtual void setLuminanceThresh(FLOAT32 luminance) = 0;
        virtual void setGammaCorrection(FLOAT32 gamma) = 0;

        virtual FLOAT32 getExposure() = 0;
        virtual FLOAT32 getLuminanceThresh() = 0;
        virtual FLOAT32 getGammaCorrection() = 0;

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

        virtual bool wasResized() = 0;

        virtual void queueLightSource(SpotLight* light) = 0;
        virtual void queueLightSource(PointLight* light) = 0;
        virtual void queueLightSource(DirectionalLight* light) = 0;
        virtual void queueRenderNode(RenderNode* node) = 0;

        virtual List<SpotLight*> &getSpotLightSources() = 0;
        virtual List<PointLight*> &getPointLightSources() = 0;
        virtual List<DirectionalLight*> &getDirectionalLightSources() = 0;
        virtual List<RenderNode*> &getRenderNodeSources() = 0;

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
