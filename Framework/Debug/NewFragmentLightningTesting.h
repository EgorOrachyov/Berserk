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
#include "Objects/Lights/GlobalLight.h"

using namespace Berserk;

class ActorCube : public Actor
{
public:

    ActorCube(const CStaticString& name, FLOAT32 lifeTime = 0) :
            Actor(name, lifeTime)
    {
        RenderMesh* mesh;
        Material* material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("Mat1"));

        Texture* diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
                 diffuse->setFiltering(GLFiltering::GLF_LINEAR, GLFiltering::GLF_LINEAR);
        Texture* specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
                 specular->setFiltering(GLFiltering::GLF_LINEAR, GLFiltering::GLF_LINEAR);
        Texture* normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_normal.jpg"), CNAME("./"));

        diffuse->setType(TextureType::TT_DIFFUSE_MAP);
        specular->setType(TextureType::TT_SPECULAR_MAP);
        normal->setType(TextureType::TT_NORMAL_MAP);

        material->setType(MaterialType::MT_DS_MAPPED);
        material->setAmbientComponent(Vector3f(0.01,0.01,0.01));
        material->setDiffuseComponent(Vector3f(0.6));
        material->setSpecularComponent(Vector3f(0.6));
        material->setShininess(6);

        material->setDiffuseMap(diffuse);
        material->setSpecularMap(specular);
        material->setNormalMap(normal);

        renderNode = gRenderSystem->createRenderNode();
        mesh = Cube::create(MeshType::MT_PNT, 1.6f, CNAME("Cube1"));
        renderNode->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1.6f, CNAME("Cube1m"));
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

class Layout : public Actor
{
public:

    Layout(const CStaticString& name) :
            Actor(name, 0)
    {
        RenderMesh* mesh;
        Material *material = gRenderSystem->getMaterialManagerPtr()->createMaterial(CNAME("LayoutMat"));

        Texture* diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_diffuse.jpg"), CNAME("./"));
        Texture* specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_specular.jpg"), CNAME("./"));
        Texture* normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("wall_normal.jpg"), CNAME("./"));

        material->setType(MaterialType::MT_DS_MAPPED);
        material->setAmbientComponent(Vector3f(0.01, 0.01, 0.01));
        material->setDiffuseComponent(Vector3f(0.9));
        material->setSpecularComponent(Vector3f(0.8));
        material->setShininess(8);

        material->setDiffuseMap(diffuse);
        material->setSpecularMap(specular);
        material->setNormalMap(normal);

        plane = gRenderSystem->createRenderNode();
        plane->setRenderNodeType(RenderNodeType::RNT_OBJECT_SHADOW);
        plane->setTransformation(translate(Vector3f(0, -3, 0)) * rotateY(Math::radians(45.0f)));
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
            mat->setDiffuseComponent(Vector3f(0.9));
            mat->setSpecularComponent(Vector3f(0.8));
            mat->setShininess(10);

            diffuse = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
            diffuse->setFiltering(GLFiltering::GLF_LINEAR, GLFiltering::GLF_LINEAR);
            specular = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_diffuse.jpg"), CNAME("./"));
            specular->setFiltering(GLFiltering::GLF_LINEAR, GLFiltering::GLF_LINEAR);
            normal = gRenderSystem->getTextureManagerPtr()->loadTexture(CNAME("box_normal.jpg"), CNAME("./"));

            mat->setDiffuseMap(diffuse);
            mat->setSpecularMap(specular);
            mat->setNormalMap(normal);

            cube[i] = gRenderSystem->createRenderNode();
            cube[i]->setRenderNodeType(RenderNodeType::RNT_OBJECT_SHADOW);
            cube[i]->setMaterial(mat);
        }

        mesh = Cube::create(MeshType::MT_PNT, 2, CNAME("Cube_0"));
        cube[0]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 2, CNAME("Cube_0m"));
        cube[0]->setShadowMesh(mesh);
        cube[0]->setTransformation(translate(Vector3f(-3,-2,-2)) * rotateY(0.5));

        mesh = Cube::create(MeshType::MT_PNT, 1, CNAME("Cube_1"));
        cube[1]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1, CNAME("Cube_1m"));
        cube[1]->setShadowMesh(mesh);
        cube[1]->setTransformation(translate(Vector3f(4,-2.5,-3)) * rotateY(0.3));

        mesh = Cube::create(MeshType::MT_PNT, 1.5, CNAME("Cube_2"));
        cube[2]->setRenderMesh(mesh);
        mesh = Cube::create(MeshType::MT_PN, 1.5, CNAME("Cube_2m"));
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
            globalLight(CNAME("DirectionalLight1")),
            actorCube(CNAME("ActorCube")),
            layout(CNAME("ActorLayout"))

    {
        which_side = true;
        sign = 1;
        camera_angle = 0;
        camera.setDirection(Vector3f(0,-0.2f,-1));
        camera.setPosition(Vector3f(0, 1, 7));
        camera.setOrientation(Vector3f(0,1,0));
        camera.setAutoAspectRatio(true);
        camera.setViewAngle(Math::radians(40.0f));
        camera.setCinematicViewport(true);
        camera.setCinematicBorder(200);
        camera.setNearClipDistance(0.001);
        camera.setFarClipDistance(100.1);

        spotLight.setDirection(Vector3f(1,-2,0));
        spotLight.setOrientation(Vector3f(2,1,0));
        spotLight.setPosition(Vector3f(-2,4,0));
        spotLight.setCutoff(Math::radians(45.0f));
        spotLight.setInnerCutoff(Math::radians(42.0f));
        spotLight.setOuterCutoff(Math::radians(46.0f));
        spotLight.setAttenuationExponent(16);
        spotLight.setLightIntensity(Vector3f(0.5));
        spotLight.setCastShadows(true);
        spotLight.setFarShadowPlane(32);

        pointLight.setPosition(Vector3f(0,4,4));
        pointLight.setRadius(16);
        pointLight.setConstantAttenuation(1);
        pointLight.setLinearAttenuation(0.03);
        pointLight.setQuadraticAttenuation(0.002);
        pointLight.setLightIntensity(Vector3f(0.5));
        pointLight.setCastShadows(true);

        globalLight.setPosition(Vector3f(0,0.5,-8));
        globalLight.setDirection(Vector3f(0,-0.5,8));
        globalLight.setOrientation(Vector3f(0,1,0));
        globalLight.setLightIntensity(Vector3f(0.75));
        globalLight.setCastShadows(true);
        globalLight.setCastLightShafts(true);
        globalLight.setSphereColor(Vector3f(1.0));
        globalLight.setBackColor(Vector3f(0.28));
        globalLight.setSphereRadius(0.03);
        globalLight.addRotation(Vector3f(1,0,0), Math::radians(4.0f));
        globalLight.addRotation(Vector3f(0,1,0), Math::radians(-90.0f));

        gRenderSystem->setClearColor(Vector4f(0.0));
        gRenderSystem->setBorderColor(Vector3f(0.0));
        gRenderSystem->setAmbientLight(Vector3f(0.16));
        //gRenderSystem->setSSAOBufferSize(0.5);
        //gRenderSystem->setSSAORadius(0.99);
        //gRenderSystem->setExposure(2.4);
        //gRenderSystem->setLuminanceThresh(0.75);
        //gRenderSystem->setGammaCorrection(2.2);
        gRenderSystem->setShadowQuality(ShadowInfo::SI_QUALITY_LOW);
        //gRenderSystem->setLightShaftsBufferSize(0.4);
        //gRenderSystem->setLightShaftsExposure(0.397);
        //gRenderSystem->setLightShaftsDecay(0.978);
        gRenderSystem->setGlobalLight(&globalLight);

        gRenderSystem->enableSSAO(false);
        gRenderSystem->enableGaussianBloom(false);
        gRenderSystem->enableToneMap(true);
        gRenderSystem->enableLightShafts(true);

        getRoot().attachActor(&camera);
        //getRoot().attachActor(&spotLight);
        //getRoot().attachActor(&pointLight);
        getRoot().attachActor(&globalLight);
        getRoot().attachActor(&actorCube);
        getRoot().attachActor(&layout);

        SceneManager::getInstancePtr()->setRenderCamera(&camera);
    }

    void onUpdate(FLOAT64 delta) override
    {
        auto scale = 0.12;

        if (which_side) camera_angle += delta * scale;
        else camera_angle -= delta * scale;
        if (camera_angle > 0.231) { which_side = false; sign = -1; }
        if (camera_angle < -0.231) { which_side = true; sign = 1; }

        globalLight.addRotation(Vector3f(0,1,0), (FLOAT32)(delta * 0.37));
        pointLight.addRotation(Vector3f(0,1,0), (FLOAT32)(delta * 0.9));
        spotLight.addRotation(Vector3f(0,1,0), (FLOAT32)(delta * 0.7));
        camera.setDirection(Vector3f(rotateY(sign * (FLOAT32)(delta * scale)) * Vector4f(camera.getDirection(), 0.0)));
    }

    virtual ~TestScene() = default;

private:

    Camera camera;
    SpotLight spotLight;
    PointLight pointLight;
    GlobalLight globalLight;
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
