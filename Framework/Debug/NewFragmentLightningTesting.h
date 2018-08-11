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

class RenderNodeActor : public Actor
{
public:

    RenderNodeActor(const CStaticString& name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime)
    {
        Material* material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("Mat1"));
        RenderMesh* mesh = gRenderSystem->getRenderMeshManagerPtr()->createRenderMesh(CNAME("Mesh1"));

        material->setType(MaterialType::MT_BASIC);
        material->setAmbientComponent(Vector3f(0.01,0.01,0.01));
        material->setDiffuseComponent(Vector3f(0.523,0.867,0.252));
        material->setSpecularComponent(Vector3f(0.523,0.867,0.252));
        material->setShininess(3);

        //      v4 ------ v7
        //     /|         /|
        //    / |        / |
        //   /  |       /  |
        //  /   v5 ----/- v6
        // v0 ------ v3   /
        // |   /      |  /
        // |  /       | /
        // | /        |/
        // v1 ------ v2

        const UINT32 vertex_count = 8;
        VertexPN v[vertex_count];

        v[0].mPosition = Vector3f(-1,1,1);    v[1].mPosition = Vector3f(-1,-1,1);
        v[0].mNormal = Vector3f(-1,1,1);      v[1].mNormal = Vector3f(-1,-1,1);
        v[0].mNormal.normalize();             v[1].mNormal.normalize();

        v[2].mPosition = Vector3f(1,-1,1);    v[3].mPosition = Vector3f(1,1,1);
        v[2].mNormal = Vector3f(1,-1,1);      v[3].mNormal = Vector3f(1,1,1);
        v[2].mNormal.normalize();             v[3].mNormal.normalize();

        v[4].mPosition = Vector3f(-1,1,-1);   v[5].mPosition = Vector3f(-1,-1,-1);
        v[4].mNormal = Vector3f(-1,1,-1);     v[5].mNormal = Vector3f(-1,-1,-1);
        v[4].mNormal.normalize();             v[5].mNormal.normalize();

        v[6].mPosition = Vector3f(1,-1,-1);   v[7].mPosition = Vector3f(1,1,-1);
        v[6].mNormal = Vector3f(1,-1,-1);     v[7].mNormal = Vector3f(1,1,-1);
        v[6].mNormal.normalize();             v[7].mNormal.normalize();



        Vector3f n0 = Vector3f(1,0,0), n1 = Vector3f(-1,0,0),
                 n2 = Vector3f(0,1,0), n3 = Vector3f(0,-1,0),
                 n4 = Vector3f(0,0,1), n5 = Vector3f(0,0,-1);

        Vector3f v0 = Vector3f(-1,1,1),  v1 = Vector3f(-1,-1,1),
                 v2 = Vector3f(1,-1,1),  v3 = Vector3f(1,1,1),
                 v4 = Vector3f(-1,1,-1), v5 = Vector3f(-1,-1,-1),
                 v6 = Vector3f(1,-1,-1), v7 = Vector3f(1,1,-1);

        const UINT32 data_count = 36;
        VertexPN data[data_count] = {
                {v0,n4},{v1,n4},{v2,n4},{v2,n4},{v3,n4},{v0,n4},
                {v3,n0},{v2,n0},{v6,n0},{v6,n0},{v7,n0},{v3,n0},
                {v7,n5},{v6,n5},{v5,n5},{v5,n5},{v4,n5},{v7,n5},
                {v4,n1},{v5,n1},{v1,n1},{v1,n1},{v0,n1},{v4,n1},
                {v4,n2},{v0,n2},{v3,n2},{v3,n2},{v7,n2},{v4,n2},
                {v6,n3},{v2,n3},{v1,n3},{v1,n3},{v5,n3},{v6,n3}
        };

        const UINT32 index_count = 36;
        UINT16  i[index_count] = {
                0,1,2,3,4,5,
                6,7,8,9,10,11,
                12,13,14,15,16,17,
                18,19,20,21,22,23,
                24,25,26,27,28,29,
                30,31,32,33,34,35
        };

        mesh->setType(MeshType::MT_PN);
        mesh->addGeometryInfo(data, data_count);
        mesh->addGeometryInfo(i, index_count);

        renderNode = gRenderSystem->createRenderNode();
        renderNode->setRenderNodeType(RenderNodeType::RNT_OBJECT);
        renderNode->setRenderMesh(mesh);
        renderNode->setMaterial(material);
        renderNode->setVisible(true);

        gRenderSystem->setAmbientLight(Vector3f(0.2));
    }


    virtual ~RenderNodeActor()
    {
        gRenderSystem->deleteRenderNode(renderNode);
    }

    void onUpdate(FLOAT64 delta) override
    {
        renderNode->setTransformation(rotate(Vector3f(0.8, 0.1, 0.63), 0.019) * renderNode->getTransformation());
        gRenderSystem->queueRenderNode(renderNode);
    }

private:

    RenderNode* renderNode;

};

class TorusActor : public Actor
{
public:

    TorusActor(const CStaticString &name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime),
            material(CNAME("BlueOcean"))
    {
        mBuffer.init();

        Torus torus;
        torus.create(1.6, 0.4, 20, 20);
        torus.fill(mBuffer);
        torus.destroy();

        material.setAmbientColor(Vector3f(0.32, 0.102, 0.86));
        material.setDiffuseColor(Vector3f(0.33, 0.11, 0.87));
        material.setSpecularColor(Vector3f(0.35, 0.13, 0.91));
        material.setShininess(18);
    }

    virtual ~TorusActor()
    {
        mBuffer.destroy();
    }

    void onUpdate(FLOAT64 delta) override
    {
        addRotation(Vector3f(0.5,0.8,0.48), 0.012);

        gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        gSceneManager->getRenderManager().queueBuffer((GPUBuffer*)&mBuffer);
        material.use();
    }

private:

    GLGPUBuffer mBuffer;
    BaseMaterial material;

};

class RenderActor : public Actor
{
public:

    RenderActor(const CStaticString &name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime),
            material(CNAME("RedMaterial"))
    {
        addMovement(Vector3f(1,0,0));
    }

    virtual ~RenderActor()
    {

    }

    void onInit() override
    {
        mBuffer1 = new GLGPUBuffer();
        mBuffer1->init();

        Sphere sphere;
        sphere.create(0.7, 32, 32);
        sphere.fill(*(GLGPUBuffer*)mBuffer1);
        sphere.destroy();

        mBuffer2 = new GLGPUBuffer();
        mBuffer2->init();

        Torus torus;
        torus.create(3, 0.7, 32, 32);
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

        gSceneManager->getRenderManager().queueTransformation(&mTransformation);
        gSceneManager->getRenderManager().queueBuffer(mBuffer1);
        material.use();

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

    TestScene(const CStaticString &name) :
            Scene(name),
            camera(CNAME("MainCamera")),
            spotLight(CNAME("SpotLight1")),
            pointLight(CNAME("PointLight1")),
            directionalLight(CNAME("DirectionalLight1")),
            renderNodeActor(CNAME("RenderNodeActorTest"))
    {
        camera.setDirection(Vector3f(0,0,-1));
        camera.setPosition(Vector3f(0, 0, 10));
        camera.setOrientation(Vector3f(0,1,0));
        camera.setAutoAspectRatio(true);
        camera.setCinematicViewport(true);
        camera.setCinematicBorder(200);
        camera.setNearClipDistance(0.001);
        camera.setFarClipDistance(100.1);

        spotLight.setDirection(Vector3f(0,0,-1));
        spotLight.setPosition(Vector3f(0,0,8));
        spotLight.setCutoff(toRadians(13.0));
        spotLight.setInnerCutoff(toRadians(7.0));
        spotLight.setOuterCutoff(toRadians(13.0));
        spotLight.setAttenuationExponent(32);
        spotLight.setLightIntensity(Vector3f(0.9,0.9,0.9));

        pointLight.setPosition(Vector3f(3,3,3));
        pointLight.setRadius(10);
        pointLight.setConstantAttenuation(1);
        pointLight.setLinearAttenuation(0.08);
        pointLight.setQuadraticAttenuation(0.005);
        pointLight.setLightIntensity(Vector3f(0.9,0.9,0.9));

        directionalLight.setDirection(Vector3f(1,0,-1));
        directionalLight.setLightIntensity(Vector3f(0.9,0.9,0.9));

        gRenderSystem->setClearColor(Vector4f(0));
        gRenderSystem->setAmbientLight(0.0);
        gRenderSystem->setExposure(4.0);
        gRenderSystem->setLuminanceThresh(0.75);
        gRenderSystem->setGammaCorrection(2.2);

        getRoot().attachActor(&camera);
        getRoot().attachActor(&spotLight);
        getRoot().attachActor(&pointLight);
        getRoot().attachActor(&directionalLight);
        getRoot().attachActor(&renderNodeActor);
    }

    virtual ~TestScene() = default;

private:

    Camera camera;
    SpotLight spotLight;
    PointLight pointLight;
    DirectionalLight directionalLight;
    RenderNodeActor  renderNodeActor;
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
