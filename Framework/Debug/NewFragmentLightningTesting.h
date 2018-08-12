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

#include "Objects/Lights/SpotLight.h"
#include "Objects/Lights/PointLight.h"
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
        material->setDiffuseComponent(Vector3f(0.523,0.9967,0.252));
        material->setSpecularComponent(Vector3f(0.1523,0.867,0.252));
        material->setShininess(13);

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
    }


    virtual ~RenderNodeActor()
    {
        gRenderSystem->deleteRenderNode(renderNode);
    }

    void onUpdate(FLOAT64 delta) override
    {
        renderNode->setTransformation(rotate(Vector3f(0.8, 0.1, 0.63), 0.029) * renderNode->getTransformation());
        gRenderSystem->queueRenderNode(renderNode);
    }

private:

    RenderNode* renderNode;

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
        //gRenderSystem->setExposure(3.5);
        //gRenderSystem->setLuminanceThresh(0.85);
        //gRenderSystem->setGammaCorrection(1.9);

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
