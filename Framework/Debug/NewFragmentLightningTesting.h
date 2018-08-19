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
        RenderMesh* mesh;
        Material* material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("Mat1"));

        Texture* diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
        Texture* specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
        Texture* normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_normal.jpg"), CNAME("./"));

        diffuse->setType(TextureType::TT_DIFFUSE_MAP);
        specular->setType(TextureType::TT_SPECULAR_MAP);
        normal->setType(TextureType::TT_NORMAL_MAP);

        material->setType(MaterialType::MT_DS_MAPPED);
        material->setAmbientComponent(Vector3f(0.01,0.01,0.01));
        material->setDiffuseComponent(Vector3f(0.8,0.8,0.8));
        material->setSpecularComponent(Vector3f(0.9,0.9,0.9));
        material->setShininess(6);

        material->setDiffuseMap(diffuse);
        material->setSpecularMap(specular);
        material->setNormalMap(normal);

        renderNode = gRenderSystem->createRenderNode();
        mesh = Cube::create(MeshType::MT_PNTBT, 1.6f, CNAME("Cube1"));
        renderNode->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1.4f, CNAME("Cube1m"));
        renderNode->setShadowMesh(mesh);
        renderNode->setMaterial(material);
        renderNode->setRenderNodeType(RenderNodeType::RNT_OBJECT_SHADOW);
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
        RenderMesh* mesh;
        Material *material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("LayoutMat"));

        Texture* diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_diffuse.jpg"), CNAME("./"));
        Texture* specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_diffuse.jpg"), CNAME("./"));
        Texture* normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_normal.jpg"), CNAME("./"));

        material->setType(MaterialType::MT_DS_MAPPED);
        material->setAmbientComponent(Vector3f(0.01, 0.01, 0.01));
        material->setDiffuseComponent(Vector3f(1.0, 1.0, 1.0));
        material->setSpecularComponent(Vector3f(1.0, 1.0, 1.0));
        material->setShininess(8);

        material->setDiffuseMap(diffuse);
        material->setSpecularMap(specular);
        material->setNormalMap(normal);

        plane = gRenderSystem->createRenderNode();
        plane->setRenderNodeType(RenderNodeType::RNT_OBJECT_SHADOW);
        plane->setTransformation(translate(Vector3f(0, -3, 0)) * rotateY(toRadians(45.0)));
        plane->setMaterial(material);
        mesh = Plane::create(MeshType::MT_PNTBT, 16, CNAME("MainPlane"));
        plane->setRenderMesh(mesh);
        mesh = Plane::create(MeshType::MT_PN, 16, CNAME("MainPlanem"));
        plane->setShadowMesh(mesh);

        for (UINT32 i = 0; i < 3; i++)
        {
            CHAR buffer[10] = "";
            sprintf(buffer, "Mat_%u", i);
            Material* mat = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME(buffer));

            mat->setType(MaterialType::MT_DS_MAPPED);
            mat->setAmbientComponent(Vector3f(0.01, 0.01, 0.01));
            mat->setDiffuseComponent(Vector3f(1.0,1.0,1.0));
            mat->setSpecularComponent(Vector3f(1.0,1.0,1.0));
            mat->setShininess(10);

            diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
            specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
            normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_normal.jpg"), CNAME("./"));

            mat->setDiffuseMap(diffuse);
            mat->setSpecularMap(specular);
            mat->setNormalMap(normal);

            cube[i] = gRenderSystem->createRenderNode();
            cube[i]->setRenderNodeType(RenderNodeType::RNT_OBJECT_SHADOW);
            cube[i]->setMaterial(mat);
        }

        mesh = Cube::create(MeshType::MT_PNTBT, 2, CNAME("Cube_0"));
        cube[0]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1.9, CNAME("Cube_0m"));
        cube[0]->setShadowMesh(mesh);
        cube[0]->setTransformation(translate(Vector3f(-3,-2,-2)) * rotateY(0.5));

        mesh = Cube::create(MeshType::MT_PNTBT, 1, CNAME("Cube_1"));
        cube[1]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 0.9, CNAME("Cube_1m"));
        cube[1]->setShadowMesh(mesh);
        cube[1]->setTransformation(translate(Vector3f(4,-2.5,-3)) * rotateY(0.3));

        mesh = Cube::create(MeshType::MT_PNTBT, 1.5, CNAME("Cube_2"));
        cube[2]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1.4, CNAME("Cube_2m"));
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
            actorCube(CNAME("ActorCube")),
            layout(CNAME("ActorLayout"))

    {
        which_side = true;
        sign = 1;
        camera_angle = 0;
        camera.setDirection(Vector3f(0,-0.3,-1));
        camera.setPosition(Vector3f(0, 3, 10));
        camera.setOrientation(Vector3f(0,1,0));
        camera.setAutoAspectRatio(true);
        camera.setCinematicViewport(true);
        camera.setCinematicBorder(200);
        camera.setNearClipDistance(0.001);
        camera.setFarClipDistance(100.1);

        spotLight.setDirection(Vector3f(0,-2,-1));
        spotLight.setOrientation(Vector3f(1,0,0));
        spotLight.setPosition(Vector3f(0,6,3));
        spotLight.setCutoff(toRadians(30.0));
        spotLight.setInnerCutoff(toRadians(26.0));
        spotLight.setOuterCutoff(toRadians(33.0));
        spotLight.setAttenuationExponent(32);
        spotLight.setLightIntensity(Vector3f(0.05,0.05,0.25));
        //spotLight.setCastShadows(true);
        spotLight.setFarShadowPlane(32);

        pointLight.setPosition(Vector3f(4,3,-1));
        pointLight.setRadius(16);
        pointLight.setConstantAttenuation(1);
        pointLight.setLinearAttenuation(0.2);
        pointLight.setQuadraticAttenuation(0.03);
        pointLight.setLightIntensity(Vector3f(0.9765,0.9765,0.9765));
        //pointLight.setCastShadows(true);

        directionalLight.setPosition(Vector3f(-5,3,0));
        directionalLight.setDirection(Vector3f(5,-2.5,0));
        directionalLight.setOrientation(Vector3f(0,1,0));
        directionalLight.setLightIntensity(Vector3f(0.261,0.061,0.061));
        //directionalLight.setCastShadows(true);

        gRenderSystem->setClearColor(Vector4f(0));
        gRenderSystem->setAmbientLight(0.08);
        gRenderSystem->setExposure(3.9);
        gRenderSystem->setLuminanceThresh(0.75);
        gRenderSystem->setGammaCorrection(2.2);
        gRenderSystem->setShadowQuality(ShadowInfo::SI_QUALITY_MEDIUM);

        getRoot().attachActor(&camera);
        getRoot().attachActor(&spotLight);
        getRoot().attachActor(&pointLight);
        getRoot().attachActor(&directionalLight);
        getRoot().attachActor(&actorCube);
        getRoot().attachActor(&layout);
    }

    void onUpdate(FLOAT64 delta) override
    {
        auto scale = 0.12;

        if (which_side) camera_angle += delta * scale;
        else camera_angle -= delta * scale;
        if (camera_angle > 0.231) { which_side = false; sign = -1; }
        if (camera_angle < -0.231) { which_side = true; sign = 1; }

        directionalLight.addRotation(Vector3f(0,1,0), (FLOAT32)(delta * 0.8));
        pointLight.addRotation(Vector3f(0,1,0), (FLOAT32)(delta * 0.9));
        camera.setDirection(Vector3f(rotateY(sign * (FLOAT32)(delta * scale)) * Vector4f(camera.getDirection(), 0.0)));
    }

    virtual ~TestScene() = default;

private:

    Camera camera;
    SpotLight spotLight;
    PointLight pointLight;
    DirectionalLight directionalLight;
    ActorCube actorCube;
    Layout layout;

    bool which_side;
    FLOAT32 sign;
    FLOAT32 camera_angle;
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
