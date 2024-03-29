#include "glad/glad.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <queue>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include "event.h"

#include "vertex.h"
#include "mesh.h"
#include "light.h"

#include "macros.h"

#include "scene.h"
#include "entity.h"
#include "event.h"

#include "texture.h"
#include "frameBuffer.h"

#include "rasterizer.h"
#include "color.h"

void Events::lightsInit(std::vector<Light>& lights)
{
    lights.push_back(Light());
}

// Don't forget the '/' at the end of the subDirectory name
Mesh* loadMeshFromObj(std::string objFile, std::string subDirectory)
{
    Mesh* meshQ = new Mesh;

    std::string inputfile = subDirectory + objFile;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), subDirectory.c_str());

    if (!warn.empty()) {
    std::cout << warn << std::endl;
    }

    if (!err.empty()) {
    std::cerr << err << std::endl;
    }

    if (!ret) {
    exit(1);
    }

    // TODO : Get multiple materials
    // for (tinyobj::material_t mat : materials)
    // {
    //     textureManager.addFromFile(mat.diffuse_texname.c_str());
    // }

    // for (size_t s = 0; s < shapes.size(); s++) 
    // {
    //     tinyobj::mesh_t& mesh = shapes[s].mesh;
    //     for (tinyobj::material_t mat : mesh.material_ids) 
    //         tinyobj::material_t& material = materials[mesh.material_ids[0]];
    // }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) 
    {
        tinyobj::mesh_t& mesh = shapes[s].mesh;
        //tinyobj::material_t& material = materials[mesh.material_ids[0]];
        // TODO : get material features
        for (unsigned int i = 0; i < attrib.vertices.size() / 3; i++)
        {
            // Set position and default uvs / color / normals
            Vertex vert;
            vert.position = Vec3(attrib.vertices[3*i], attrib.vertices[3*i+1], attrib.vertices[3*i+2]);
            vert.normal = Vec3(0,0,1);
            vert.u = 0;
            vert.v = 1;
            vert.color = Color(1,1,1,1);

            meshQ->vertices.emplace_back(vert);
        }

        for (unsigned int i = 0; i < mesh.indices.size(); i++)
        {
            tinyobj::index_t idx = mesh.indices[i];
            meshQ->indices.emplace_back(idx.vertex_index);

            // Loads normals if possible
            if (idx.normal_index >= 0)
            {
                const tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                const tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                const tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];

                meshQ->vertices[idx.vertex_index].normal = Vec3(nx, ny, nz);
            }

            // Loads uvs if possible
            if (idx.texcoord_index >= 0)
            {
                const tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                const tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

                meshQ->vertices[idx.vertex_index].u = tx;
                meshQ->vertices[idx.vertex_index].v = ty;
            }
        }
    }
    return meshQ;
}

// Spawns every entity of the scene
void Events::entitiesInit(std::vector<Entity>& entities)
{
    float zDepth = -5;

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(vertex.position.x, vertex.position.y, vertex.position.z);
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-2.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateSphere(15, 15);
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(vertex.position.x, vertex.position.y, vertex.position.z);
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(2.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f, 1.f, 1.f));
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }

    zDepth -= 15;

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-2.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
        sphere.mesh->pTexture = &textureManager.textures[2];
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateSphere(15, 15);

        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(2.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f, 1.f, 1.f));
        sphere.mesh->pTexture = &textureManager.textures[2];
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }

    zDepth -= 20;

    {
        for (unsigned int j = 0; j < 2; j++)
        {
            Entity sphere;
            sphere.mesh = Mesh::CreateCube();
            for (Vertex& vertex : sphere.mesh->vertices)
            {
                vertex.color = Color(1, 1, 1);
            }
            sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-2.0-j*3, 0.0, zDepth));
            sphere.transformation *= Mat4::CreateRotationMatrix(Vec3(j, j, j));
            sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
            sphere.mesh->pTexture = &textureManager.textures[1];
            sphere.alpha = 1.f;
            entities.push_back(std::move(sphere));
        }
    }

    {
        for (unsigned int j = 0; j < 2; j++)
        {
            Entity sphere;
            sphere.mesh = Mesh::CreateSphere(15, 15);

            sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(2.0+j*3, 0.0, zDepth));
            sphere.transformation *= Mat4::CreateRotationMatrix(Vec3(j, j, j));
            sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f, 1.f, 1.f));
            sphere.mesh->pTexture = &textureManager.textures[1];
            sphere.alpha = 1.f;
            entities.push_back(std::move(sphere));
        }
    }

    zDepth -= 20;
    
    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-7.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
        sphere.mesh->pTexture = &textureManager.textures[2];
        sphere.alpha = 0.8f;
        entities.push_back(std::move(sphere));
    }

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateSphere(15, 15);

        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(7.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f, 1.f, 1.f));
        sphere.mesh->pTexture = &textureManager.textures[2];
        sphere.alpha = 0.8f;
        entities.push_back(std::move(sphere));
    }

    for (unsigned int i = 0; i < 2; i++)
    {
        Entity wall;
        wall.mesh = Mesh::CreateCube();
        for (Vertex& vertex : wall.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
        }
        wall.transformation *= Mat4::CreateTranslationMatrix(Vec3(15.0 * pow((-1), i), 0.0, zDepth));
        wall.transformation *= Mat4::CreateScaleMatrix(Vec3(1.0, 5.5, 5.5));
        wall.mesh->pTexture = &textureManager.textures[1];
        wall.alpha = 1.0f;
        entities.push_back(std::move(wall));
    }

    zDepth -= 20;

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-7.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
        sphere.mesh->pTexture = &textureManager.textures[1];
        sphere.alpha = 0.2f;
        entities.push_back(std::move(sphere));
    }

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateSphere(15, 15);

        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(7.0, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f, 1.f, 1.f));
        sphere.mesh->pTexture = &textureManager.textures[1];
        sphere.alpha = 0.2f;
        entities.push_back(std::move(sphere));
    }

    for (unsigned int i = 0; i < 2; i++)
    {
        {
            Entity wall;
            wall.mesh = Mesh::CreateCube();
            for (Vertex& vertex : wall.mesh->vertices)
            {
                vertex.color = Color(1, 1, 1);
            }
            wall.transformation *= Mat4::CreateTranslationMatrix(Vec3(15.0 * pow((-1), i), 0.0, zDepth));
            wall.transformation *= Mat4::CreateScaleMatrix(Vec3(1.0, 5.5, 5.5));
            wall.mesh->pTexture = &textureManager.textures[2];
            wall.alpha = 1.0f;
            entities.push_back(std::move(wall));
        }
    }

    zDepth -= 20;

    {
        Entity sphere;
        sphere.mesh = loadMeshFromObj("Boo.obj", "media/SuperMarioGalaxyBoo/");
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.1, 0.0, zDepth));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f/50.f, 1.f/50, 1.f/50));
        sphere.mesh->pTexture = &textureManager.textures[5];
        sphere.alpha = 1.0f;
        entities.push_back(std::move(sphere));
    }

    // {
    //     Entity sphere;
    //     sphere.mesh = loadMeshFromObj("waddledee.obj", "media/WaddleDeeLow-Poly/");
    //     sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(5.0, -2.0, zDepth));
    //     sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f/50.f, 1.f/50, 1.f/50));
    //     sphere.mesh->pTexture = &textureManager.textures[4];
    //     sphere.alpha = 1.f;
    //     entities.push_back(std::move(sphere));
    // }
}

void Events::sceneInit(Scene& scene)
{
    lightsInit(scene.lights);
    entitiesInit(scene.entities);
}

Events::Events()
{
    #ifdef __GLFW__
    if (!glfwInit())
    {
        bRun = false;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(windowWidth, windowHeight, "Geometry", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        glfwTerminate();
        bRun = false;
        return;
    }

    // Set F1 key function
    f1.onSwitch = [&](bool isOn)
    {
        if (isOn)
        {
            renderMode = E_RasterizerMode::E_WIREFRAME;
        }
        else 
        {
            renderMode = E_RasterizerMode::E_TRIANGLES;
        }
    };

    //hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
}

Events::~Events()
{
    // clean
    #ifdef __GLFW__
    glfwDestroyWindow(window);
    glfwTerminate();
    #endif
}

int Events::run()
{
    //============== init scene ==============//
    if (!bRun)
        return EXIT_FAILURE;

    sceneInit(scene);

    FrameBuffer target(textureResolutionX, textureResolutionY);

    //============== setup fps and deltatime ==============//
    float lastTime = 0.f;
    #ifdef __FPS_COUNT__
    unsigned int nbFps = 0;
    float totalFps = 0.f;
    float frame = 0;
    float fps;
    constexpr long unsigned int nbDeltaTimeSamples = 10;
    std::array<float, nbDeltaTimeSamples> deltaTimes;
    unsigned int deltaTimeIndex = 0;
    float lowestFPS = 10000;
    float highestFPS = 0.f;
    float deltaMedium = 1;
    #endif

    //============== start game loop ==============//
    #ifdef __GLFW__
    while (bRun && !glfwWindowShouldClose(window))
    #endif
    {
        #ifdef __GLFW__
        //generate inputs
        glfwPollEvents();
        //clear buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        #endif

        //============== compute deltatime ==============//
        float time = glfwGetTime();
        float deltaTime = time - lastTime;
        #ifdef __FPS_COUNT__
        deltaTimes[deltaTimeIndex] = deltaTime;
        deltaTimeIndex++;
        if (deltaTimeIndex >= nbDeltaTimeSamples)
        {
            deltaTimeIndex = 0;

            deltaMedium = 0.f;
            for (float f : deltaTimes)
            {
                deltaMedium += f;
            }
            deltaMedium /= deltaTimes.size();
            lowestFPS = std::min(deltaMedium, lowestFPS);
            highestFPS = std::max(deltaMedium, highestFPS);
        }

        fps = 1.f/(deltaTime);
        nbFps++;
        totalFps += fps;
        // std::cout << 1.f/(deltaTime) << std::endl;
        lastTime = time;
        #endif

        #ifdef __FPS_COUNT__
        frame += 1;
        #endif

        //============== Add rotation to cube ==============//

        scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        scene.entities[1].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));

        // set additional shaders
        scene.entities[1].mat.additionalShaders = [&time](Color& color, Vec3& worldLocation)
        {
            constexpr float minColorRatio = 0.0; 
            // add green circles on xy plane which are twinkling
            color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10) * (cos(time) + 1 + minColorRatio);
        };

        //GLFW
        #ifdef __GLFW__

        #ifdef __THIRD_PERSON__
        // TODO : add spherical coordinates and TPS

        // if (glfwGetKey(window, GLFW_KEY_W))
        //     camera.spherialRadius -= camera.translationSpeed * deltaTime;

        // if (glfwGetKey(window, GLFW_KEY_S))
        //     camera.spherialRadius += camera.translationSpeed * deltaTime;
        
        // if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        //     glfwSetWindowShouldClose(window, GL_TRUE);
        // if (glfwGetKey(window, GLFW_KEY_UP))
        //     camera.sphericalRotation += Vec3(camera.rotationSpeed*deltaTime,0,0);
        // if (glfwGetKey(window, GLFW_KEY_DOWN))
        //     camera.sphericalRotation += Vec3(-camera.rotationSpeed*deltaTime, 0, 0);
        // if (glfwGetKey(window, GLFW_KEY_LEFT))
        //     camera.sphericalRotation += Vec3(0,camera.rotationSpeed*deltaTime,0);
        // if (glfwGetKey(window, GLFW_KEY_RIGHT))
        //     camera.sphericalRotation += Vec3(0,-camera.rotationSpeed*deltaTime,0);
        #endif

        //============== INPUTS ==============//

        // call camera inputs
        camera.inputs(deltaTime, window);

        // stops loop if ESCAPE key is pressed down
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            bRun = false;

        f1.input(glfwGetKey(window, GLFW_KEY_F1));
        
        if (glfwGetKey(window, GLFW_KEY_F2))
            scene.lights[0].position = camera.cartesianLocation;

        #endif

        //============== Add rotation to cube ==============//

        camera.actualize();

        Rasterizer::RenderScene(&scene, &target, 
            Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0.05, 2, 90), 
            camera.getTransform().GetInverse(), camera, renderMode);
        
        #ifdef __GLFW__
        #ifndef __ANTI_ALIASING__
        glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_FLOAT, target.texture.pixels);
        #else
        Texture renderedTexture(windowWidth, windowHeight);
        Rasterizer::antiAliasingCompression(target, renderedTexture);
        glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_FLOAT, renderedTexture.pixels);
        #endif

        glfwSwapBuffers(window);

        #endif
    }

    return EXIT_SUCCESS;
}

