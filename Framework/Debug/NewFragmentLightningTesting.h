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
#include "Shapes/Cube.h"
#include "Shapes/Plane.h"

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

class ActorCube : public Actor
{
public:

    ActorCube(const CStaticString& name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime)
    {
        RenderMesh* mesh = Cube::create(MeshType::MT_PN, 1.6f, CNAME("Cube1"));
        Material* material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("Mat1"));

        material->setType(MaterialType::MT_BASIC);
        material->setAmbientComponent(Vector3f(0.01,0.01,0.01));
        material->setDiffuseComponent(Vector3f(0.523,0.9967,0.252));
        material->setSpecularComponent(Vector3f(0.9,0.867,0.9));
        material->setShininess(6);

        renderNode = gRenderSystem->createRenderNode();
        renderNode->setRenderMesh(mesh);
        renderNode->setShadowMesh(mesh); mesh->addReference();
        renderNode->setMaterial(material);
        renderNode->setRenderNodeType(RenderNodeType::RNT_OBJECT);
    }


    virtual ~ActorCube()
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

class ActorPlane : public Actor
{
public:

    ActorPlane(const CStaticString& name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime)
    {
        RenderMesh* mesh = Plane::create(MeshType::MT_PN, 10, CNAME("MainPlane"));
        Material* material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("Mat2"));

        material->setType(MaterialType::MT_BASIC);
        material->setAmbientComponent(Vector3f(0.01,0.01,0.01));
        material->setDiffuseComponent(Vector3f(0.5523,0.29967,0.9252));
        material->setSpecularComponent(Vector3f(0.9,0.867,0.9));
        material->setShininess(3);

        renderNode = gRenderSystem->createRenderNode();
        renderNode->setRenderMesh(mesh);
        renderNode->setMaterial(material);
        renderNode->setRenderNodeType(RenderNodeType::RNT_OBJECT);
        renderNode->setTransformation(translate(Vector3f(0,-3,0)));
    }


    virtual ~ActorPlane()
    {
        gRenderSystem->deleteRenderNode(renderNode);
    }

    void onUpdate(FLOAT64 delta) override
    {
        gRenderSystem->queueRenderNode(renderNode);
    }

private:

    RenderNode* renderNode;

};

class Layout : public Actor
{
public:

    Layout(const CStaticString& name) :
            Actor(name, 0)
    {
        RenderMesh* mesh = Plane::create(MeshType::MT_PN, 20, CNAME("MainPlane"));
        Material *material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("LayoutMat"));

        material->setType(MaterialType::MT_BASIC);
        material->setAmbientComponent(Vector3f(0.01, 0.01, 0.01));
        material->setDiffuseComponent(Vector3f(0.523, 0.741, 0.537));
        material->setSpecularComponent(Vector3f(0.1523, 0.867, 0.752));
        material->setShininess(10);

        plane = gRenderSystem->createRenderNode();
        plane->setRenderNodeType(RenderNodeType::RNT_OBJECT);
        plane->setTransformation(translate(Vector3f(0, -3, 0)) * rotateY(0.733));
        plane->setMaterial(material);
        plane->setRenderMesh(mesh);
        plane->setShadowMesh(mesh); mesh->addReference();

        for (UINT32 i = 0; i < 3; i++)
        {
            CHAR buffer[10] = "";
            sprintf(buffer, "Mat_%u", i);
            Material* mat = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME(buffer));

            mat->setType(MaterialType::MT_BASIC);
            mat->setAmbientComponent(Vector3f(0.01, 0.01, 0.01));
            mat->setDiffuseComponent(Vector3f(0.323 * i + 0.4, 0.456 + 0.2741 * i, 0.337 * (3 - i)));
            mat->setSpecularComponent(Vector3f(0.1523 * i + 0.23, 0.867, 0.752));
            mat->setShininess(10);

            cube[i] = gRenderSystem->createRenderNode();
            cube[i]->setRenderNodeType(RenderNodeType::RNT_OBJECT);
            cube[i]->setMaterial(mat);
        }

        mesh = Cube::create(MeshType::MT_PN, 2, CNAME("Cube_0"));
        mesh->addReference();
        cube[0]->setRenderMesh(mesh);
        cube[0]->setShadowMesh(mesh);
        cube[0]->setTransformation(translate(Vector3f(-3,-2,-2)) * rotateY(0.5));

        mesh = Cube::create(MeshType::MT_PN, 1, CNAME("Cube_1"));
        mesh->addReference();
        cube[1]->setRenderMesh(mesh);
        cube[1]->setShadowMesh(mesh);
        cube[1]->setTransformation(translate(Vector3f(4,-2.5,-3)) * rotateY(0.3));

        mesh = Cube::create(MeshType::MT_PN, 1.5, CNAME("Cube_2"));
        mesh->addReference();
        cube[2]->setRenderMesh(mesh);
        cube[2]->setShadowMesh(mesh);
        cube[2]->setTransformation(translate(Vector3f(0.5,-2.25,-6)) * rotateY(0.94));
    }

    ~Layout()
    {
        gRenderSystem->deleteRenderNode(plane);
        for (auto &i : cube)
        {
            gRenderSystem->deleteRenderNode(i);
        }
    }

    void onUpdate(FLOAT64 delta)
    {
        gRenderSystem->queueRenderNode(plane);
        for (auto &i : cube)
        {
            gRenderSystem->queueRenderNode(i);
        }
    }

private:

    RenderNode* plane;
    RenderNode* cube[3];

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
            renderNodeActor(CNAME("RenderNodeActorTest")),
            actorCube(CNAME("ActorCube")),
            layout(CNAME("ActorLayout"))

    {
        camera.setDirection(Vector3f(0,-0.3,-1));
        camera.setPosition(Vector3f(0, 3, 10));
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

        pointLight.setPosition(Vector3f(3,3,5));
        pointLight.setRadius(20);
        pointLight.setConstantAttenuation(1);
        pointLight.setLinearAttenuation(0.08);
        pointLight.setQuadraticAttenuation(0.005);
        pointLight.setLightIntensity(Vector3f(0.765,0.765,0.765));

        directionalLight.setPosition(Vector3f(-7,3,10));
        directionalLight.setDirection(Vector3f(0.5,-0.3,-1));
        directionalLight.setOrientation(Vector3f(0,1,0));
        directionalLight.setCastShadows(true);
        directionalLight.setLightIntensity(Vector3f(0.949,0.49,0.49));

        gRenderSystem->setClearColor(Vector4f(0));
        gRenderSystem->setAmbientLight(0.08);
        //gRenderSystem->setExposure(3.5);
        //gRenderSystem->setLuminanceThresh(0.85);
        //gRenderSystem->setGammaCorrection(1.9);

        getRoot().attachActor(&camera);
        getRoot().attachActor(&spotLight);
        getRoot().attachActor(&pointLight);
        getRoot().attachActor(&directionalLight);
        //getRoot().attachActor(&renderNodeActor);
        getRoot().attachActor(&actorCube);
        getRoot().attachActor(&layout);
    }

    void onUpdate(FLOAT64 delta) override
    {
        pointLight.addRotation(Vector3f(0,1,0), 0.028);
    }

    virtual ~TestScene() = default;

private:

    Camera camera;
    SpotLight spotLight;
    PointLight pointLight;
    DirectionalLight directionalLight;
    RenderNodeActor  renderNodeActor;
    ActorCube actorCube;
    Layout layout;
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
