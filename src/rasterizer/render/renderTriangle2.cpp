#include "renderTriangle2.h"

#include "entity.h"
#include "camera.h"
#include "rasterizer.h"
#include "mesh.h"

void RenderTriangle2::projectAndDraw(std::vector<Light>& lights, const Entity* entity,
                                     FrameBuffer* pTarget, const Mat4& screenConversionMatrix, 
                                     Camera& camera, E_RasterizerMode mode, std::array<float, 3>& w)
{
    // Convert vertices coordinates to the user Viewport
    setVerticesToScreenResolution(screenConversionMatrix);

    // Mix entity and vertex transparency together. 
    addTransparency(entity->alpha);

    switch (mode)
    {
        case E_RasterizerMode::E_WIREFRAME:
        case E_RasterizerMode::E_TRIANGLES_AS_LINES:
            drawWireframe(pTarget);
            break;

        case E_RasterizerMode::E_TRIANGLES:
            drawTriangleX(pTarget, w, camera.cartesianLocation, lights, entity->mesh->pTexture, entity->mat);   
            break; 

        default:
            break;
    }
}

bool tryToDrawPixel(unsigned int x, unsigned int y, float depth, 
                           FrameBuffer* pTarget, std::function<Color(void)>& getColor)
{   
    //BECAREFUL
    float currentDepth = pTarget->depthBuffer.getDepth(x, y);

    if (currentDepth <= depth)
        return false;

    pTarget->SetPixel(x, y, depth, getColor());

    return true;
}