#include "glad/glad.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <queue>

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include "event.h"

#include "sdlUtilities.h"

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

// Mesh* loadMeshFromObj()
// {
//     Mesh* mesh = new Mesh;

//     // Vertex vert1 (Vec3(0,0,0), Vec3(0,0,1), Color(1.f, 1.f, 1.f, 1.f));
//     // vert1.u = 0.f;
//     // vert1.v = 0.f;
//     // mesh->vertices.emplace_back(vert1);
//     // Vertex vert2 (Vec3(1,1,0), Vec3(0,0,1), Color(1.f, 1.f, 1.f, 1.f));
//     // vert2.u = 0.f;
//     // vert2.v = 1.f;
//     // mesh->vertices.emplace_back(vert2);
//     // Vertex vert3 (Vec3(1,0,0), Vec3(0,0,1), Color(1.f, 1.f, 1.f, 1.f));
//     // vert3.u = 1.f;
//     // vert3.v = 1.f;
//     // mesh->vertices.emplace_back(vert3);

//     // mesh->indices.emplace_back(0);
//     // mesh->indices.emplace_back(1);
//     // mesh->indices.emplace_back(2);

//     // return mesh;

// 	// Initialize Loader
// 	objl::Loader Loader;

// 	// Load .obj File
// 	bool loadout = Loader.LoadFile("media/midna.obj");

// 	// Check to see if it loaded

// 	// If so continue
// 	if (loadout)
// 	{
// 		// Create/Open e1Out.txt
// 		std::ofstream file("e1Out.txt");

// 		// Go through each loaded mesh and out its contents
// 		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
// 		{
// 			// Copy one of the loaded meshes to be our current mesh
// 			objl::Mesh curMesh = Loader.LoadedMeshes[i];

// 			// Print Mesh Name
// 			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

// 			// Print Vertices
// 			file << "Vertices:\n";

// 			// Go through each vertex and print its number,
// 			//  position, normal, and texture coordinate
// 			for (int j = 0; j < curMesh.Vertices.size(); j++)
// 			{
//                 Vertex v;
//                 v.position = Vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
//                 v.normal   = Vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
//                 v.u = curMesh.Vertices[j].TextureCoordinate.X;
//                 v.v = curMesh.Vertices[j].TextureCoordinate.Y;
//                 v.color = {1.f, 1.f, 1.f, 1.f};

// 				file << "V" << j << ": " <<
// 					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
// 					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
// 					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";

//                 mesh->vertices.emplace_back(v);
// 			}

// 			// Print Indices
// 			file << "Indices:\n";

// 			// Go through every 3rd index and print the
// 			//	triangle that these indices represent
// 			for (int j = 0; j < curMesh.Indices.size(); j += 3)
// 			{
// 				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
// 			    mesh->indices.emplace_back(curMesh.Indices[j]);
// 			    mesh->indices.emplace_back(curMesh.Indices[j+1]);
// 			    mesh->indices.emplace_back(curMesh.Indices[j+2]);
//             }

// 			// Print Material
// 			file << "Material: " << curMesh.MeshMaterial.name << "\n";
// 			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
// 			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
// 			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
// 			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
// 			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
// 			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
// 			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
// 			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
// 			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
// 			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
// 			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
// 			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

// 			// Leave a space to separate from the next mesh
// 			file << "\n";
// 		}

// 		// Close File
// 		file.close();
//         return mesh;
// 	}
// 	// If not output an error
// 	else
// 	{
// 		// Create/Open e1Out.txt
// 		std::ofstream file("e1Out.txt");

// 		// Output Error
// 		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

// 		// Close File
// 		file.close();
//         return nullptr;
// 	}
// }

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
        for (unsigned int i = 0; i < attrib.vertices.size(); i++)
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
        //sphere.mesh->pTexture = &textureManager.textures[1];
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
        //sphere.mesh->pTexture = &textureManager.textures[1];
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }

    zDepth -= 15;

    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        // float ii = 0;
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
            //ii += 255.f / 20*20;
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

    // {
    //     Entity sphere;
    //     sphere.mesh = Mesh::CreateCube();
    //     for (Vertex& vertex : sphere.mesh->vertices)
    //     {
    //         vertex.color = Color(1, 1, 1);
    //     }
    //     sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, 0.0, zDepth));
    //     sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(5.5, 5.5, 1.5));
    //     sphere.alpha = 1.0f;
    //     entities.push_back(std::move(sphere));
    // }
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
                //ii += 255.f / 20*20;
            }
            sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, -1.0, zDepth));
            sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.f/10.f, 1.f/10, 1.f/10));
            sphere.mesh->pTexture = &textureManager.textures[0];
            sphere.alpha = 1.f;
            entities.push_back(std::move(sphere));
        }
    }

    // {
    //     Entity sphere;
    //     //sphere.mesh = Mesh::CreateSphere(15, 15);
    //     sphere.mesh = loadMeshFromObj();

    //     sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, 0.0, 0.0));
    //     sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
    //     sphere.mesh->pTexture = &textureManager.textures[0];
    //     sphere.alpha = 1.f;
    //     entities.push_back(std::move(sphere));
    // }
}

void Events::sceneInit(Scene& scene)
{
    lightsInit(scene.lights);
    entitiesInit(scene.entities);
}

void Events::inputs(SDL_Event& event)
{
    //inputs
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                //cameraInputs(event.key.keysym.sym);
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        bRun = false;
                        break;
                    case SDLK_F1 :
                        //F1.input(true);
                        renderMode = E_RasterizerMode::E_WIREFRAME;
                        break;
                    
                }
                break;

            default: break;
        }
    }
}

Events::Events()
#ifdef __SDL__
    : render(SDL_Utilities(bRun))
#endif
{
    // F1.onSwitch = [&](bool isOn)
    // {
    //     if (isOn)
    //         renderMode = E_RasterizerMode::E_WIREFRAME;
    //     else
    //         renderMode = E_RasterizerMode::E_TRIANGLES;
    // };

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
    //camera = Mat4::CreateTranslationMatrix(Vec3(0,0,10));
}

Events::~Events()
{
    
}

__inline
float lerp(float a, float b, float f)
{
    return a + f * (b - a);
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

    //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.00, 0.00, PI/5));
    #ifdef __SDL__
    while (bRun)
    #endif
    #ifdef __GLFW__
    while (bRun && !glfwWindowShouldClose(window))
    #endif
    {
        #ifdef __GLFW__
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        #endif

        float time = float (SDL_GetTicks()) / 1000.f;
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
        // std::cout << deltaMedium << '\n';

        fps = 1.f/(deltaTime);
        nbFps++;
        totalFps += fps;
        std::cout << 1.f/(deltaTime) << std::endl;
        lastTime = time;
        //std::cout << totalFps / nbFps << std::endl;

        frame += 1;

        scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        scene.entities[1].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        //scene.entities[1].transformation = Mat4::CreateTranslationMatrix(camera.cartesianLocation / 2);

        //scene.entities[1].transformation = Mat4::CreateTranslationMatrix(Vec3(camera.cartesianLocation.x, -10, camera.cartesianLocation.z));

        //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.03, 0.03, 0.03));
        // for (Entity& entity : scene.entities)
        //     scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.03, 0.03, 0.03));
            //entity.transformation = Mat4::CreateTranslationMatrix(Vec3(100 * sin(frame/50), 0.00, 0));

        //scene.entities[0].transformation = Mat4::CreateTranslationMatrix(Vec3(3.32 * sin(frame/40), 0.00, 0));

        // scene.lights[0].position.x = 10 * sin(frame/10);
        // scene.lights[0].position.y = 10 * cos(frame/10);

        // scene.entities[1].mat.additionalShaders = [&deltaMedium, &highestFPS, &lowestFPS](Color& color, Vec3& worldLocation)
        // {
        //     // color.r /= (sin(worldLocation.x * 10) + PI) * 0.8; 
        //     // if (color.r > 1.f)
        //     //     color.r = 1.f;

        //     // const float delta = (sin(worldLocation.x * 10) / PI + 1) * 0.8;
        //     // color.r += (sin(worldLocation.x * 10) / PI + 1) * 0.8;
        //     // color.g += (cos(worldLocation.y * 10) / PI + 1) * 0.8;
        //     // color.b += 0;

        //     // if ((int((worldLocation.x / 1)) + int((worldLocation.y / 1)) + int((worldLocation.z / 1))) % 2 == 0)
        //     // if ((int(floor(worldLocation.x / 1)) + int(floor(worldLocation.y / 1)) + int(floor(worldLocation.z / 1))) % 2 == 0)
        //     //     color.r -= 0.2;
        //     // color.r = (deltaMedium - lowestFPS) / (highestFPS - lowestFPS) * 1;//sin(frame/10);
        //     // color.r = 1 - (highestFPS - lowestFPS) / (lerp(lowestFPS, highestFPS, deltaMedium) - lowestFPS);
        //     // std::cout << color.r << '\n';
        //     color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10);

        //     // if (color.r < 0)
        //     //     color.r = 0;
        // };

        //SDL
        inputs(event);

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

        //double xpos, ypos;
        //glfwGetCursorPos(window, &xpos, &ypos);
//
        //constexpr const double mouseSensibility = 2.f;
//
        //if (lastYPos - ypos < - mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(-0.1, 0, 0));
        //if (lastYPos - ypos > mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.1, 0, 0));
//
        //if (lastXPos - xpos < - mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));
        //if (lastXPos - xpos > mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));
//
        //lastXPos = xpos;
        //lastYPos = ypos;
        #endif

        camera.actualize();

        // rasterizer
        // Rasterizer::RenderScene(&scene, &target, 
        //     Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0, 2, 1.4), 
        //     E_RasterizerMode::E_TRIANGLES);
        //Rasterizer::RenderScene(&scene, &target, Mat4::CreateOrthogonalProjectionMatrix(), camera.GetInverse(), renderMode);
        Rasterizer::RenderScene(&scene, &target, 
            Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0.05, 2, 90), 
            camera.getTransform().GetInverse(), camera, renderMode);

        // render.SDL_RenderTexture(target.texture);
        
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

        #ifdef __SDL__
        render.SDL_RenderTexture(target.texture);
        #endif
    }

    #ifdef __GLFW__
    glfwDestroyWindow(window);
    glfwTerminate();
    #endif

    return EXIT_SUCCESS;
}