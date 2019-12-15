#include "renderTriangle2.h"

#include "entity.h"
#include "camera.h"
#include "rasterizer.h"
#include "mesh.h"

void RenderTriangle2::projectAndDraw(std::vector<Light>& lights, const Entity* entity,
                                     FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& screenConversionMatrix, 
                                     Camera& camera, E_RasterizerMode mode)
{
    std::array<float, 3> w = projectVertices(projectionMatrix);

    setVerticesToScreenResolution(screenConversionMatrix);

    //rendering.setDefaultColor();

    addTransparency(entity->alpha);

    switch (mode)
    {
        case E_RasterizerMode::E_WIREFRAME:
        case E_RasterizerMode::E_TRIANGLES_AS_LINES:
            drawWireframe(pTarget);
            break;

        // rendering.v1.color = Color(250,250,250);   
        // rendering.v2.color = Color(250,100,250);   
        // rendering.v3.color = Color(250,250,100);

        case E_RasterizerMode::E_TRIANGLES:
            drawTriangleX(pTarget, w, camera.cartesianLocation, lights, entity->mesh->pTexture, entity->mat);   
            break; 

        default:
            break;
    }
}