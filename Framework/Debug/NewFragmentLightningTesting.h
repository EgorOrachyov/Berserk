//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_NEWFRAGMENTLIGHTNINGTESTING_H
#define BERSERKENGINE_NEWFRAGMENTLIGHTNINGTESTING_H

#include "Context/ApplicationContext.h"
#include "Managers/SceneManager.h"

#include "Strings/UtilityString.h"
#include "Shapes/Sphere.h"
#include "Shapes/Torus.h"

#include "Objects/Materials/BaseMaterial.h"
#include "Objects/Lights/SpotLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/AmbientLight.h"
#include "Objects/Lights/DirectionalLight.h"

using namespace Berserk;

class RenderActor : public Actor
{
public:

    RenderActor(const CStaticString &name, FLOAT32 lifeTime = 0) : Actor(name, lifeTime), material(CNAME("RedMaterial"))
    {
        addMovement(Vector3f(1,0,0));
    }

    virtual ~RenderActor()
    {

    }

    void onInit() override
    {
        mBuffer1 = new GLGPUBuffer(CNAME("SphereBuffer1"));
        mBuffer1->init();

        Sphere sphere;
        sphere.create(1, 32, 32);
        sphere.fill(*(GLGPUBuffer*)mBuffer1);
        sphere.destroy();

        mBuffer2 = new GLGPUBuffer(CNAME("SphereBuffer2"));
        mBuffer2->init();

        Torus torus;
        torus.create(2, 0.8, 32, 32);
        torus.fill(*(GLGPUBuffer*)mBuffer2);
        torus.destroy();

        material.setAmbientColor(Vector3f(0.92, 0.102, 0.16));
        material.setDiffuseColor(Vector3f(0.9, 0.11, 0.17));
        material.setSpecularColor(Vector3f(0.92, 0.872, 0.886));
        material.setShininess(6);
    }

    void onUpdate(FLOAT64 delta) override
    {
        addRotation(Vector3f(0.5,0.8,0.48), 0.018);

        //gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        //gSceneManager->getRenderManager().queueBuffer(mBuffer1);
        //material.use();

        gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        gSceneManager->getRenderManager().queueBuffer(mBuffer2);
        material.use();
    }

    void onDestroy() override
    {
        SAFE_DELETE(mBuffer1);
        SAFE_DELETE(mBuffer2);
    }

private:

    GPUBuffer *mBuffer1;
    GPUBuffer *mBuffer2;
    BaseMaterial material;

};

class TestScene : public Scene
{
public:

    TestScene(const CStaticString &name) : Scene(name),
                                           camera(CNAME("MainCamera")),
                                           renderActor(CNAME("TestActor")),
                                           spotLight(CNAME("SpotLight1")),
                                           pointLight(CNAME("PointLight1")),
                                           ambientLight(CNAME("AmbientLight1")),
                                           directionalLight(CNAME("DirectionalLight1"))
    {
        camera.setDirection(Vector3f(0,0,-1));
        camera.setPosition(Vector3f(0, 0, 10));
        camera.setOrientation(Vector3f(0,1,0));
        camera.setAutoAspectRatio(true);
        camera.setCinematicViewport(true);
        camera.setCinematicBorder(200);

        ambientLight.setLightIntensity(Vector3f(0.064, 0.064, 0.064));

        spotLight.setDirection(Vector3f(0,0,-1));
        spotLight.setPosition(Vector3f(0,0,8));
        spotLight.setCutoff(toRadians(13.0));
        spotLight.setInnerCutoff(toRadians(7.0));
        spotLight.setOuterCutoff(toRadians(13.0));
        spotLight.setAttenuationExponent(32);
        spotLight.setLightIntensity(Vector3f(0.91,0.870,0.940));

        pointLight.setPosition(Vector3f(3,3,3));
        pointLight.setRadius(10);
        pointLight.setConstantAttenuation(1);
        pointLight.setLinearAttenuation(0.08);
        pointLight.setQuadraticAttenuation(0.005);
        pointLight.setLightIntensity(Vector3f(0,0.1,0));

        directionalLight.setDirection(Vector3f(1,0,-1));
        directionalLight.setLightIntensity(Vector3f(0,0,0.1));

        getRoot().attachActor(&camera);
        getRoot().attachActor(&renderActor);
        getRoot().attachActor(&spotLight);
        getRoot().attachActor(&ambientLight);
        getRoot().attachActor(&pointLight);
        getRoot().attachActor(&directionalLight);
    }

    virtual ~TestScene() = default;

private:

    Camera camera;
    RenderActor renderActor;
    AmbientLight ambientLight;
    SpotLight spotLight;
    PointLight pointLight;
    DirectionalLight directionalLight;
};

class TestApplication : public ApplicationContext
{
public:

    void setup() override
    {
        testScene = new TestScene(CNAME("GeometryTestScene"));
        getSceneManager()->addScene(testScene, true);
    }

    void close() override
    {
        SAFE_DELETE(testScene);
    }

private:

    TestScene *testScene;

};

void NewFragLightTesting()
{
    TestApplication application;

    application.init();
    application.run();
    application.destroy();
}

#endif //BERSERKENGINE_NEWFRAGMENTLIGHTNINGTESTING_H
