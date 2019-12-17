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

Mesh* loadMeshFromObj(RessourceManager& textureManager)
{
    Mesh* meshQ = new Mesh;

    std::string inputfile = "media/midna/midna.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), "media/midna/");

    if (!warn.empty()) {
    std::cout << warn << std::endl;
    }

    if (!err.empty()) {
    std::cerr << err << std::endl;
    }

    if (!ret) {
    exit(1);
    }

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
        tinyobj::material_t& material = materials[mesh.material_ids[0]];
        // material.diffuse_texname;
        // textureManager.textures.emplace_back(std::move(Texture("media/midna/midona_body.png")));
        for (unsigned int i = 0; i < attrib.vertices.size() / 3; i++)
        {
            Vertex vert;
            vert.position = Vec3(attrib.vertices[3*i], attrib.vertices[3*i+1], attrib.vertices[3*i+2]);
            // vert.normal   = Vec3(attrib.normals[i], attrib.normals[i+1], attrib.normals[i+2]);
            vert.normal = Vec3(0,0,1);
            vert.u = 0;
            vert.v = 0.5;
            vert.color = Color(1,1,1,1);

            meshQ->vertices.emplace_back(vert);
        }

        for (unsigned int i = 0; i < shapes[s].mesh.indices.size(); i++)
        {
            tinyobj::index_t idx = shapes[s].mesh.indices[i];
            meshQ->indices.emplace_back(idx.vertex_index);

            if (idx.normal_index >= 0)
            {
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];

                meshQ->vertices[idx.vertex_index].normal = Vec3(nx, ny, nz);
            }

            tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
            tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

            meshQ->vertices[idx.vertex_index].u = tx;
            meshQ->vertices[idx.vertex_index].v = ty;
        }


        // // Loop over faces(polygon)
        // size_t index_offset = 0;
        // for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
        // {
        //     int fv = shapes[s].mesh.num_face_vertices[f];

        //     // Loop over vertices in the face.
        //     for (size_t v = 0; v < fv; v++) 
        //     {
        //         // access to vertex
        //         tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        //         tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
        //         tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
        //         tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
        //         tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
        //         tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
        //         tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
        //         tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
        //         tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];

        //         // Optional: vertex colors
        //         // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
        //         // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
        //         // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
        //     }
        //     index_offset += fv;

        //     // per-face material
        //     shapes[s].mesh.material_ids[f];
        // }
    }
    return meshQ;
}

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
            // float ii = 0;
            for (Vertex& vertex : sphere.mesh->vertices)
            {
                vertex.color = Color(1, 1, 1);
                //ii += 255.f / 20*20;
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
        // float ii = 0;
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
            //ii += 255.f / 20*20;
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
        for (unsigned int j = 0; j < 1; j++)
        {
            Entity sphere;
            //sphere.mesh = Mesh::CreateSphere(15, 15);
            sphere.mesh = loadMeshFromObj(textureManager);
            // float ii = 0;
            for (Vertex& vertex : sphere.mesh->vertices)
            {
                vertex.color = Color(1, 1, 1);
            }
            sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, -1.0, zDepth));
            sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f/10.f, 1.f/10, 1.f/10));
            sphere.mesh->pTexture = &textureManager.textures[0];
            sphere.alpha = 1.f;
            entities.push_back(std::move(sphere));
        }
    }
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

    //hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
}

Events::~Events()
{
    
}

int Events::run()
{
    if (!bRun)
        return EXIT_FAILURE;

    sceneInit(scene);

    FrameBuffer target(textureResolutionX, textureResolutionY);
    //Texture target(windowWidth, windowHeight);

    unsigned int nbFps = 0;
    float totalFps = 0.f;
    float frame = 0;
    float fps;
    float lastTime = 0.f;
    constexpr long unsigned int nbDeltaTimeSamples = 10;
    std::array<float, nbDeltaTimeSamples> deltaTimes;
    unsigned int deltaTimeIndex = 0;
    float lowestFPS = 10000;
    float highestFPS = 0.f;
    float deltaMedium = 1;

    #ifdef __GLFW__
    while (bRun && !glfwWindowShouldClose(window))
    #endif
    {
        #ifdef __GLFW__
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        #endif

        float time = glfwGetTime();
        float deltaTime = time - lastTime;
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
        std::cout << 1.f/(deltaTime) << std::endl;
        lastTime = time;

        frame += 1;

        scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        scene.entities[1].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));

        scene.entities[1].mat.additionalShaders = [&deltaMedium, &highestFPS, &lowestFPS](Color& color, Vec3& worldLocation)
        {
            color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10);
        };

        //GLFW
        #ifdef __GLFW__

        #ifdef __THIRD_PERSON__
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

        camera.inputs(deltaTime, window);

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

        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            bRun = false;

        f1.input(glfwGetKey(window, GLFW_KEY_F1));
        
        if (glfwGetKey(window, GLFW_KEY_F2))
            scene.lights[0].position = camera.cartesianLocation;

        #endif

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

    #ifdef __GLFW__
    glfwDestroyWindow(window);
    glfwTerminate();
    #endif

    return EXIT_SUCCESS;
}