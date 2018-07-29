//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_LAYOUTAPPLICATIONRENDERTESTING_H
#define BERSERKENGINE_LAYOUTAPPLICATIONRENDERTESTING_H

#include "Context/ApplicationContext.h"
#include "Managers/SceneManager.h"
#include "Strings/UtilityString.h"
#include "Shapes/Sphere.h"
#include "Objects/Lights/PointLight.h"

using namespace Berserk;

class myActor : public Actor
{
public:

    myActor(const CStaticString &name, FLOAT32 lifeTime = 0) : Actor(name, lifeTime)
    {

    }

    virtual ~myActor()
    {
        SAFE_DELETE(mBuffer);
        SAFE_DELETE(camera);
        SAFE_DELETE(pointLight);
    }

    void onInit() override;

    void onUpdate(FLOAT64 delta) override
    {
        gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        gSceneManager->getRenderManager().queueBuffer(mBuffer);
        gSceneManager->getRenderManager().queueMaterial(&materialComponent);
    }

private:

    GPUBuffer *mBuffer;
    BaseMaterialComponent materialComponent;
    PointLightComponent pointLightComponent;
    CameraComponent cameraComponent;

    Camera *camera;
    PointLight *pointLight;

};

void myActor::onInit()
{
    mBuffer = new GLGPUBuffer(CNAME("SphereBuffer"));
    mBuffer->init(); PUSH("Calls me");
    Sphere mSphere;
    mSphere.create(1.0, 32, 32);
    mSphere.fill(*dynamic_cast<GLGPUBuffer*>(mBuffer));
    mSphere.destroy();

    Vector3f Ka(0.02, 0.02, 0.02);
    Vector3f Kd(0.9, 0.11, 0.12);
    Vector3f Ks(0.9, 0.11, 0.12);
    FLOAT32  sh(8);

    materialComponent.mAmbient = Ka;
    materialComponent.mDiffuse = Kd;
    materialComponent.mSpecular = Ks;
    materialComponent.mShininess = sh;

    pointLightComponent.mCastShadows = false;
    pointLightComponent.mLightIntensity = Vector3f(1,1,1);
    pointLightComponent.mPosition = Vector4f(2, 2, 2, 1);

    cameraComponent.mView = lookAt(Vector3f(0, 5, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    cameraComponent.mProjection = perspective((FLOAT32)toRadians(50), 1, 0.1, 100);

    camera = new Camera(CNAME("MyCamera"));
    pointLight = new PointLight(CNAME("MyPointLight"));

    camera->setDirection(Vector3f(0,-1,-1));
    camera->setPosition(Vector3f(0, 5, 5));
    camera->setOrientation(Vector3f(0,1,0));

    pointLight->setPosition(Vector3f(4,4,4));
    pointLight->setLightIntensity(Vector3f(1,1,1));
    pointLight->setCastShadows(false);

    gSceneManager->getCurrentScene()->getRoot().attachActor(camera);
    gSceneManager->getCurrentScene()->getRoot().attachActor(pointLight);
}

class myApp : public ApplicationContext
{
public:

    void setup() override
    {
        scene = new Scene(CNAME("TestScene1"));
        actor = new myActor(CNAME("RenderSphere"));
        scene->getRoot(SI_LAYOUT_0).attachActor(actor);
        gSceneManager->addScene(scene, true);
    }

    void close() override
    {
        SAFE_DELETE(actor);
        SAFE_DELETE(scene);
    }

private:

    myActor *actor;
    Scene *scene;

};

void myAppTesting()
{
    myApp application;

    application.init();
    application.run();
    application.destroy();
}

#endif //BERSERKENGINE_LAYOUTAPPLICATIONRENDERTESTING_H
