//
// Created by Egor Orachyov on 29.07.2018.
//

#ifndef BERSERKENGINE_LAYOUTAPPLICATIONRENDERTESTING_H
#define BERSERKENGINE_LAYOUTAPPLICATIONRENDERTESTING_H

#include "Context/ApplicationContext.h"
#include "Managers/SceneManager.h"
#include "Strings/UtilityString.h"
#include "Shapes/Sphere.h"

using namespace Berserk;

class myActor : public Actor
{
public:

    myActor(const CStaticString &name, FLOAT32 lifeTime = 0) : Actor(name, lifeTime)
    {
        mBuffer = new GLGPUBuffer(CNAME("SphereBuffer"));
        mBuffer->init();
        mSphere.create(1.0, 32, 32);
        mSphere.fill(*dynamic_cast<GLGPUBuffer*>(mBuffer));

        Vector3f Ka(0.25, 0.02, 0.02);
        Vector3f Kd(0.9, 0.11, 0.12);
        Vector3f Ks(0.9, 0.41, 0.52);
        FLOAT32  sh(16);

        materialComponent.mAmbient = Ka;
        materialComponent.mDiffuse = Kd;
        materialComponent.mSpecular = Ks;
        materialComponent.mShininess = sh;

        pointLightComponent.mCastShadows = false;
        pointLightComponent.mLightIntensity = Vector3f(1,1,1);
        pointLightComponent.mPosition = Vector4f(2, 2, 2, 1);

        cameraComponent.mView = lookAt(Vector3f(0, 5, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
        cameraComponent.mProjection = perspective((FLOAT32)toRadians(50), 1, 0.1, 100);

        addMovement(Vector3f(2,0,0));
    }

    virtual ~myActor()
    {
        SAFE_DELETE(mBuffer);
    }

    void onUpdate(FLOAT64 delta) override
    {
        addRotation(Vector3f(0,1,0), 0.02);

        gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        gSceneManager->getRenderManager().queueBuffer(mBuffer);
        gSceneManager->getRenderManager().queueMaterial(&materialComponent);
        gSceneManager->getRenderManager().queueLight(&pointLightComponent);
        gSceneManager->getRenderManager().queueCamera(&cameraComponent);
    }

private:

    GPUBuffer *mBuffer;
    Sphere mSphere;
    BaseMaterialComponent materialComponent;
    PointLightComponent pointLightComponent;
    CameraComponent cameraComponent;

};

class myApp : public ApplicationContext
{
public:

    void setup() override
    {
        actor = new myActor(CNAME("RenderSphere"));
        scene = new Scene(CNAME("TestScene1"));

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
