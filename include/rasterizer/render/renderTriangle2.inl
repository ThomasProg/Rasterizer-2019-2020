#include "renderTriangle2.h"

#include "vec4.h"
#include "mat4.h"
#include "macros.h"
#include "light.h"
#include "texture.h"
#include "color.h"
#include "frameBuffer.h"
#include "vec2.h"

__inline
void RenderTriangle2::setupForTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    this->triangleVertices = {v1, v2, v3};
}

__inline
bool RenderTriangle2::isBackFaceCulled(const Vec3& cameraLocation)
{
    //is it really performant to compute normals for every triangle?
    //couldn't we do in another way, with already computed normals?

    //compute plane normal
    Vec3 N = crossProduct(Vec3((triangleVertices[1].position - triangleVertices[0].position)), 
                          Vec3((triangleVertices[2].position - triangleVertices[0].position)));

    worldVertices[0] = triangleVertices[0].position;
    worldVertices[1] = triangleVertices[1].position;
    worldVertices[2] = triangleVertices[2].position;

    return dotProduct(N, triangleVertices[0].position - cameraLocation) > 0;
}

namespace triangles
{
    __inline
    void clipHorizontal(float& x1, float& y1, const Vec2& vec2, float limit)
    {
        const float var = -x1 + limit;

        x1 += var;
        y1 += var * vec2.y / vec2.x;
    }

    __inline
    void clipVertical(float& x1, float& y1, const Vec2& vec2, float limit)
    {
        const float var = -y1 + limit;

        y1 += var;
        x1 += var * vec2.x / vec2.y;
    }
};

__inline
bool RenderTriangle2::isClipped(const Texture&,// pTarget, 
                                std::vector<RenderTriangle2>&,// additionalTriangles,
                                std::array<Vec4, 3>& projectedTriangles)
{
    // for each vertex
    for (const Vec4& vertex : projectedTriangles)
    {
        if (vertex.w <= 0 || vertex.w <= 0 || vertex.w <= 0)
            return true;

        #ifdef __W_CLIPPING__
        // for each coordinate (x, y)
        for (unsigned int j = 0; j < 2; j++)
        {
            // w is mulitplied by 2 because of the CreateScreenConversionMatrix().
            if (vertex.arr[j] > vertex.w*2 || vertex.arr[j] < - vertex.w*2)
            {
                // TODO : For each other vertex, call ClipHorizontal and ClipVertical 
                //        and store the vertices into additionalTriangles.
                //        This clipping should also be done for z.

                // for (const Vec4& otherVertex : projectedTriangles)
                // {
                //     if (&vertex != &otherVertex)
                //     {
                //         triangles::clipHorizontal(vertex.x, vertex.y, 
                //                                   Vec2(vertex.x - otherVertex.x, vertex.y - otherVertex.y), 
                //                                   vertex.w*2);

                //         triangles::clipVertical(vertex.x, vertex.y, 
                //                                 Vec2(vertex.x - otherVertex.x, vertex.y - otherVertex.y), 
                //                                 vertex.w*2);
                //     }
                // }
                return true;
            }
            // else 
            //     std::cout << vertex.arr[j] << '\n';
        }
        #endif
    }

    return false;

    float& x1 = triangleVertices[0].position.x;
    float& y1 = triangleVertices[0].position.y;

    float& x2 = triangleVertices[1].position.x;
    float& y2 = triangleVertices[1].position.y;

    // float& x3 = triangleVertices[2].position.x;
    // float& y3 = triangleVertices[2].position.y;

    float limit = 1.0;

    if (x1 > limit)
        triangles::clipHorizontal(x1, y1, 
                                Vec2(x2-x1, y2-y1), 
                                limit);
                                
    // TODO: add triangles with clipping
    return false;
}

__inline
void RenderTriangle2::setVerticesToScreenResolution(const Mat4& screenConversion)
{
    triangleVertices[0].position = screenConversion * Vec4(triangleVertices[0].position, 1);
    triangleVertices[1].position = screenConversion * Vec4(triangleVertices[1].position, 1);
    triangleVertices[2].position = screenConversion * Vec4(triangleVertices[2].position, 1);
}

__inline
void RenderTriangle2::addTransparency(const float transparency)
{
    triangleVertices[0].color.a *= transparency;
    triangleVertices[1].color.a *= transparency;
    triangleVertices[2].color.a *= transparency;
}

namespace lines
{
    __inline
    void clipHorizontal(int& x1, int& y1, int x2, int y2, Vec2& vec2, float& magnitude, float limit)
    {
        const float var = -x1 + limit;

        x1 += var;
        y1 += var * vec2.y / vec2.x;

        vec2 = Vec2(x2-x1, y2-y1);
        magnitude = vec2.GetMagnitude();
        vec2.toUnit();
    }


    __inline
    void clipHorizontalOtherPoint(int x1, int y1, int& x2, int& y2, Vec2& vec2, float& magnitude, float limit)
    {
        const float var = -x2 + limit;

        x2 += var;
        y2 += var * vec2.y / vec2.x;
        
        vec2 = Vec2(x2-x1, y2-y1);
        magnitude = vec2.GetMagnitude();
        vec2.toUnit();
    }


    __inline
    void clipVertical(int& x1, int& y1, int x2, int y2, Vec2& vec2, float& magnitude, float limit)
    {
        const float var = -y1 + limit;

        y1 += var;
        x1 += var * vec2.x / vec2.y;

        vec2 = Vec2(x2-x1, y2-y1);
        magnitude = vec2.GetMagnitude();
        vec2.toUnit();
    }


    __inline
    void clipVerticalOtherPoint(int x1, int y1, int& x2, int& y2, Vec2& vec2, float& magnitude, float limit)
    {
        const float var = -y2 + limit;

        y2 += var;
        x2 += var * vec2.x / vec2.y;
        
        vec2 = Vec2(x2-x1, y2-y1);
        magnitude = vec2.GetMagnitude();
        vec2.toUnit();
    }
};

__inline
void RenderTriangle2::drawLineX(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2)
{
    //get distance between two points
    int x1 = vertex1.position.x;
    int y1 = vertex1.position.y;
    int x2 = vertex2.position.x;
    int y2 = vertex2.position.y;
    
    #ifdef __CLIP_WIREFRAME_LINES__
    if (x1 < windowRenderMinX && x2 < windowRenderMinX) //TODO : if cross screen
        return;

    if (x1 > windowRenderMaxX && x2 > windowRenderMaxX) //TODO : if cross screen
        return;

    if (y1 < windowRenderMinY && y2 < windowRenderMinY) //TODO : if cross screen
        return;

    if (y1 > windowRenderMaxY && y2 > windowRenderMaxY) //TODO : if cross screen
        return;
    #endif

    Vec2 vec2(x2-x1, y2-y1);
    //get distance between the 2 points
    float magnitude = vec2.GetMagnitude();

    if (magnitude != 0.f)
    {
        vec2.toUnit();

        #ifdef __CLIP_WIREFRAME_LINES__
        //clip left for x1
        if (x1 < windowRenderMinX)
        {
            lines::clipHorizontal(x1, y1, x2, y2, vec2, magnitude, windowRenderMinX);
        }

        //clip left for x2
        if (x2 < windowRenderMinX)
        {
            lines::clipHorizontalOtherPoint(x1, y1, x2, y2, vec2, magnitude, windowRenderMinX);
        }

        //clip right for x1
        if (x1 > windowRenderMaxX)
        {
            lines::clipHorizontal(x1, y1, x2, y2, vec2, magnitude, windowRenderMaxX);
        }

        //clip right for x2
        if (x2 > windowRenderMaxX)
        {
            lines::clipHorizontalOtherPoint(x1, y1, x2, y2, vec2, magnitude, windowRenderMaxX);
        }

        //clip up for y1
        if (y1 < windowRenderMinY)
        {
            lines::clipVertical(x1, y1, x2, y2, vec2, magnitude, windowRenderMinY);
        }

        //clip up for y2
        if (y2 < windowRenderMinY)
        {
            lines::clipVerticalOtherPoint(x1, y1, x2, y2, vec2, magnitude, windowRenderMinY);
        }

        //clip down for y1
        if (y1 > windowRenderMaxX)
        {
            lines::clipVertical(x1, y1, x2, y2, vec2, magnitude, windowRenderMaxY);
        }

        //clip down for y2
        if (y2 > windowRenderMaxX)
        {
            lines::clipVerticalOtherPoint(x1, y1, x2, y2, vec2, magnitude, windowRenderMaxY);
        }

        #endif

        // to prevent "infinite" loop error
        #ifndef __CLIP_WIREFRAME_LINES__
        if (magnitude < 1000)
        #endif
        for (float i = 0; i <= magnitude; ++i)
        {
            float ratio = i / magnitude; 
            Vec3 point(x1 + i * vec2.x, 
                       y1 + i * vec2.y, 
                       vertex1.position.z * ratio + vertex2.position.z * (1 - ratio));

            Color c(1,1,1);

            if (point.x >= 0 && point.x < pTarget->width && point.y >= 0 && point.y < pTarget->height)
                pTarget->SetPixel(point.x, point.y, point.z, c);
        }
    }
    else 
        if (x2 >= 0 && x2 < int(pTarget->width) && y2 >= 0 && y2 < int(pTarget->height))
            pTarget->SetPixel(x2, y2, vertex2.position.z, vertex1.color);
}

__inline 
void RenderTriangle2::drawWireframe(FrameBuffer* pTarget)
{
    // Draw a line for each edge of the triangle.
    drawLineX(pTarget, triangleVertices[0], triangleVertices[1]);
    drawLineX(pTarget, triangleVertices[1], triangleVertices[2]);
    drawLineX(pTarget, triangleVertices[2], triangleVertices[0]);
}

inline
void getWeight(const Vec2& p, const Vec3& p1, const Vec3& p2, const Vec3& p3, 
                std::array<float, 3>& weight)
{
    float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    if (det == 0)
    {
        weight[0] = 0;
        weight[1] = 0;
        weight[2] = 0;
    }

    weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
    weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
    weight[2] = 1 - weight[1] - weight[0];
}

//12 operations
inline
void getWeight(const float pxp3x, const float pyp3y, 
                std::array<float, 3>& weight, 
                const weightPrecomputedData& precomputed)
{
    // float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    weight[0] = (precomputed.p2p3y * pxp3x + precomputed.p3p2x * pyp3y) / precomputed.det;
    weight[1] = (precomputed.p1p3x * pyp3y - precomputed.p1p3y * pxp3x) / precomputed.det;
    weight[2] = 1 - weight[1] - weight[0];
}

// BECAREFUL
// loop could be unrolled without this function
// This function can interpolate arrays
template<typename T, typename U, long unsigned int N>
U operator*(const std::array<T, N>& lhs, const std::array<U, N>& rhs)
{
    U result = 0.f;
    for (unsigned int i = 0; i < N; ++i)
    {
        result += lhs[i] * rhs[i];
    }
    return result;
}

__inline
void getTextureColor(Texture const * const pTexture, 
                     std::array<Vertex, 3>& triangleVertices, 
                     std::array<float, 3>& weight, 
                     std::array<float, 3>& uP, 
                     std::array<float, 3>& vP, 
                     Color& c)
{
    if (pTexture != nullptr && pTexture->pixels != nullptr)
    {
        // could be unrolled
        float u = weight * uP;
        float v = weight * vP; 

        // make uv range from 0 to 1    
        u = fmod(u, 1);
        if (u < 0)
            u += 1;
        v = fmod(v, 1);
        if (v < 0)
            v += 1;

        //TODO : enum for interpolation type
        #ifdef __NEAREST_INTERPOLATION__
        {
            getTexturedColorNearestInterpolation(pTexture, 
                                                u, v, c);
        }
        #endif

        #ifdef __BILINEAR_INTERPOLATION__
        {
            getTexturedColorBilinearInterpolation(pTexture, 
                                                u, v, c);
        }
        #endif

        c.a += weight[0] * triangleVertices[0].color.a
            + weight[1] * triangleVertices[1].color.a
            + weight[2] * triangleVertices[2].color.a;
    }
    #if defined(__NEAREST_INTERPOLATION__) || defined(__BILINEAR_INTERPOLATION__)
    else 
    #endif
        getUntexturedPixelColor(triangleVertices, weight, c);
}

__inline
void RenderTriangle2::drawTriangleX(FrameBuffer* pTarget, std::array<float, 3>& ww, 
                                    const Vec3& cameraLocation, std::vector<Light>& lights, 
                                    Texture* pTexture, const Material& mat)
{

    // should not happen, but we guard it 
    #ifdef __PERSPECTIVE_FIX__
    // TODO: assert?
    // if ww == 0, it should have been clipped already
    if (ww[0] == 0 || ww[1] == 0 || ww[2] == 0)
        return;

    #endif

    //getting aliases
    const Vec3& p1 = triangleVertices[0].position;
    const Vec3& p2 = triangleVertices[1].position;
    const Vec3& p3 = triangleVertices[2].position;

    //BECAREFUL
    //clipping in rasterization 

    // We get the AABB box of the triangle.
    // We clamp it to not go outside of the screen. 
    #ifdef __ADD_OFFSET__
    unsigned int minX = std::min(std::max(windowRenderMinX, std::min(std::min(p1.x, p2.x), p3.x)), windowRenderMaxX);
    unsigned int maxX = std::min(std::max(windowRenderMinX, std::max(std::max(p1.x, p2.x), p3.x)), windowRenderMaxX);
    unsigned int minY = std::min(std::max(windowRenderMinY, std::min(std::min(p1.y, p2.y), p3.y)), windowRenderMaxY);
    unsigned int maxY = std::min(std::max(windowRenderMinY, std::max(std::max(p1.y, p2.y), p3.y)), windowRenderMaxY);
    #else
    unsigned int minX = std::min(std::max(0.f, std::min(std::min(p1.x, p2.x), p3.x)), float(pTarget->width));
    unsigned int maxX = std::min(std::max(0.f, std::max(std::max(p1.x, p2.x), p3.x)), float(pTarget->width));
    unsigned int minY = std::min(std::max(0.f, std::min(std::min(p1.y, p2.y), p3.y)), float(pTarget->height));
    unsigned int maxY = std::min(std::max(0.f, std::max(std::max(p1.y, p2.y), p3.y)), float(pTarget->height));
    #endif

    // Moving uvs into arrays for future uses and better manipulation.
    // v = 1-v to mirror them on the x axis, since they are upside down by default.
    std::array<float, 3> uP = {triangleVertices[0].u, triangleVertices[1].u, triangleVertices[2].u};
    std::array<float, 3> vP = {1-triangleVertices[0].v, 1-triangleVertices[1].v, 1-triangleVertices[2].v};

    // Array containing the point weight relative to the triangle.
    std::array<float, 3> weight;

    std::function<Color(void)> getColor;

    // For Multi Sampling,
    // getLight() is called only one time for multiple pixels.
    // that is why we have a boolean checking if it is a new sample or a new pixel.
    #ifdef __MULTI_SAMPLING_LIGHT__
    bool bComputed = false;
    float currentLuminosity = 0.f;

    std::function<float(const Vec3&, 
                        const Vec3&, 
                        const std::vector<Light>&, 
                        const Vec3&, 
                        const Material&)> 
        getLight = [&bComputed, &currentLuminosity](const Vec3& location3D, 
                                                    const Vec3& normal, 
                                                    const std::vector<Light>& lights, 
                                                    const Vec3& cameraLocation, const Material& mat)
    {
        if (!bComputed)
        {        
            bComputed = true;
            currentLuminosity = getPixelLight(location3D, normal, lights, cameraLocation, mat);
        }
        return currentLuminosity;
    };
    #endif

    // Some parts of Phong and Blinn Phong should be put in a function together.

    #ifdef __BLINN_PHONG_LIGHTING__
    // get Color, relative to normals, and texture
    getColor = [&](void)
    {
        Vec3 location3D = weight * worldVertices;

        Vec3 normal(0, 0, 0);
        // computes normal
        // could be unrolled
        for (unsigned int i = 0; i < 3; i++)
        {
            normal += weight[i] * triangleVertices[i].normal;
        }

        #ifdef __PERSPECTIVE_FIX__
        {
            weight[0] /= ww[0];
            weight[1] /= ww[1];
            weight[2] /= ww[2];

            float total = weight[0] + weight[1] + weight[2];
            if (total == 0)
                return Color(0, 0, 0, 0);
                
            weight[0] /= total;
            weight[1] /= total;
            weight[2] /= total;
        }
        #endif

        Color c(0, 0, 0, 0);
        
        // lighting
        float intensity;

        normal.Normalize();

        #ifdef __MULTI_SAMPLING_LIGHT__
        intensity = getLight(location3D, normal, lights, cameraLocation, mat);
        #else
        // BLINN PHONG
        intensity = getPixelLight(location3D, normal, lights, cameraLocation, mat);
        #endif

        getTextureColor(pTexture, 
                        triangleVertices, 
                        weight, 
                        uP, 
                        vP, 
                        c);

        // Texture we can set on top of other textures, 
        // with absolute coordinates.
        #ifdef __ADDITIONAL_SHADERS__
        Vec3 newLoc = weight * worldVertices;
        mat.additionalShaders(c, newLoc);
        #endif


        #ifdef __WIREFRAME_ON_TRIANGLES__

        if (   weight[0] < wireframeTriangleSize
            || weight[1] < wireframeTriangleSize
            || weight[2] < wireframeTriangleSize)
        {
            c.r = 0.5f;
            c.g = 0.f;
            c.b = 0.5f; 
        }
        #endif

        // add intensity
        c.r *= intensity;
        c.g *= intensity;
        c.b *= intensity;

        return c;
    };

    #endif

    #if defined(__PHONG_LIGHTING__)

    // Get light intensity for the 3 vertices of the triangle
    std::array<float, 3> intensity;
    for (unsigned int i = 0; i < 3; ++i)
    {
        intensity[i] = getPixelLight(worldVertices[i], triangleVertices[i].normal, lights, cameraLocation, mat);
    }

    // Set getColor function
    // using phong
    getColor = [&](void)
    {
        Color c(0, 0, 0, 0);

        #ifdef __PERSPECTIVE_FIX__
        {
            weight[0] /= ww[0];
            weight[1] /= ww[1];
            weight[2] /= ww[2];

            float total = weight[0] + weight[1] + weight[2];
            if (total == 0)
                return Color(0, 0, 0, 0);
                
            weight[0] = weight[0] / total;
            weight[1] = weight[1] / total;
            weight[2] = weight[2] / total;
        }
        #endif

        getTextureColor(pTexture, 
                        triangleVertices, 
                        weight, 
                        uP, 
                        vP, 
                        c);

<<<<<<< HEAD
        // Interpolates finalIntensity
=======
>>>>>>> 8bf52768a8107cb847a58d3cac810625a4416227
        float finalIntensity = 0.f;
        for (unsigned int i = 0; i < 3; ++i)
        {
            finalIntensity += intensity[i] * weight[i];
        }

        if (finalIntensity > 1)
            finalIntensity = 1;

        else if (finalIntensity < 0)
            finalIntensity = 0;

        #ifdef __ADDITIONAL_SHADERS__
        Vec3 newLoc = weight * worldVertices;
        mat.additionalShaders(c, newLoc);
        #endif

        // Add Intensity
        c.r *= finalIntensity;
        c.g *= finalIntensity;
        c.b *= finalIntensity;

        return c;
    };
    #endif

    const weightPrecomputedData weightData (p2.y - p3.y, 
                                            p1.y - p3.y,
                                            p3.x - p2.x,
                                            p1.x - p3.x);

    //TODO : set WeightVar outside of the loops
    #if defined(__MULTI_SAMPLING_LIGHT__)
    for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
    {
        for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
        {
    #else 
    for (unsigned int y = minY; y <= maxY; y ++)
    {
        for (unsigned int x = minX; x <= maxX; x ++)
        {
    #endif
            #ifdef __MULTI_SAMPLING_LIGHT__
<<<<<<< HEAD
            bComputed = false; // Tells light has to be computed again since it is a new pixel
=======
            bComputed = false;
>>>>>>> 8bf52768a8107cb847a58d3cac810625a4416227
            //for each sample
            for (unsigned int yAliasing = 0; yAliasing < antiAliasingY; yAliasing ++)
            {
                for (unsigned int xAliasing = 0; xAliasing < antiAliasingX; xAliasing ++)
                {
                    // draw pixel
                    getWeight(x + xAliasing - triangleVertices[2].position.x, 
                              y + yAliasing - triangleVertices[2].position.y, 
                              weight, weightData);

                    // If point is inside triangle
                    if (weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0)
                    {
                        // Interpolates depth
                        const float depth = (triangleVertices[0].position.z) * weight[0] 
                                        + (triangleVertices[1].position.z) * weight[1] 
                                        + (triangleVertices[2].position.z) * weight[2];

                        tryToDrawPixel(x + xAliasing, y + + yAliasing, depth, pTarget, getColor);
                    }
                }
            }
            
            #else

            getWeight(x - triangleVertices[2].position.x, y - triangleVertices[2].position.y, weight, weightData);

            // If point is inside triangle
            if (weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0)
            {
                // Interpolates depth
                const float depth = (triangleVertices[0].position.z) * weight[0] 
                                  + (triangleVertices[1].position.z) * weight[1] 
                                  + (triangleVertices[2].position.z) * weight[2];

                tryToDrawPixel(x, y, depth, pTarget, getColor);
            }

            #endif
        }
    }
}



__inline
void getUntexturedPixelColor(const std::array<Vertex, 3>& triangleVertices, 
                             const std::array<float, 3>& weight, 
                             Color& colorOutput)
{
    // Could be unrolled for optimization
    // interpolate 
    for (unsigned int i = 0; i < 3; i++)
    {
        colorOutput.r += weight[i] * triangleVertices[i].color.r;
        colorOutput.g += weight[i] * triangleVertices[i].color.g;
        colorOutput.b += weight[i] * triangleVertices[i].color.b;
        colorOutput.a += weight[i] * triangleVertices[i].color.a;
    }
}

__inline
void getTexturedColorNearestInterpolation(const Texture* texture, 
                                          const float u,
                                          const float v, 
                                          Color& colorOutput)
{
    // u and v represents a pixel location in the  texture, and them must be between 0 and 1.
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);
 
    // Max uv is 1, 1 * width = width, width isn't a valid index, 
    // so we substract by 1 before the multiplication.
    // since (u, v) >= 0, we can cast them to unsigned int without errors.
    colorOutput.copyRGB(texture->GetPixelColor(static_cast<unsigned int>(u * (float(texture->width))), 
                                               static_cast<unsigned int>(v * (float(texture->height)))));
}

__inline
void getTexturedColorBilinearInterpolation(const Texture* texture, 
                                           const float u,
                                           const float v, 
                                           Color& colorOutput)
{
    // floor
    // x1,y1--------x2,y1
    //   |            |
    //   |            |
    //   |  x,y       |
    //   |   #        |
    //   |            |
    // x1,y2--------x2,y2
    //               ceil

    // u and v represents a pixel location in the  texture, and them must be between 0 and 1.
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);

    const unsigned int x1 = static_cast<unsigned int>(floor(u * (float(texture->width)-1)));
    const unsigned int x2 = std::min(static_cast<unsigned int>(ceil(u * (float(texture->width)-1))), texture->width - 1);

    const unsigned int y1 = static_cast<unsigned int>(floor(v * (float(texture->height)-1)));
    const unsigned int y2 = std::min(static_cast<unsigned int>(ceil(v * (float(texture->height)-1))), texture->height - 1);

    const float curX = u * (float(texture->width) - 1);
    const float curY = v * (float(texture->height) - 1);

    //can't interpolate in case interpolating points are the same
    if (x2 == x1 && y2 == y1)
    {
        colorOutput.reset();
        return;
    }
    //in case the two interpolating points are on the same horizontal line
    else if (x2 == x1)
    {
        const Color c1 = texture->GetPixelColor(x1, y1);
        const Color c2 = texture->GetPixelColor(x1, y2);

        colorOutput.copyRGB(getAverageColor(c2, c1, (curY - y1) / (y2 - y1)));
    }
    //in case the two interpolating points are on the same vertical line
    else if (y2 == y1)
    {
        const Color c1 = texture->GetPixelColor(x1, y1);
        const Color c2 = texture->GetPixelColor(x2, y1);

        colorOutput.copyRGB(getAverageColor(c2, c1, (curX - x1) / (x2 - x1)));
    }
    //general case : 
    else 
    {
        const Color c11 = texture->GetPixelColor(x1, y1);
        const Color c21 = texture->GetPixelColor(x2, y1);
        const Color c12 = texture->GetPixelColor(x1, y2);
        const Color c22 = texture->GetPixelColor(x2, y2);

        const Color c1 = getAverageColor(c21, c11, (curX - x1 / (x2 - x1)));
        const Color c2 = getAverageColor(c22, c12, (curX - x1 / (x2 - x1)));

        colorOutput.copyRGB(getAverageColor(c2, c1, (curY - y1) / (y2 - y1)));
    }
}

__inline
float getPixelLight(const Vec3& location3D, 
                                    const Vec3& normal, 
                                    const std::vector<Light>& lights, 
                                    const Vec3& cameraLocation, const Material& mat)
{
    float ambient  = 0.f;
    float diffuse  = 0.f;
    float specular = 0.f;  
    float total    = 0.f;

    for (const Light& light : lights)
    {
        //ambient
        ambient = light.ambientComponent * mat.ambient;

        //diffuse
        Vec3 pixelToLightVec = (light.position - location3D);
        pixelToLightVec.Normalize();
        
        float cosTeta = std::max(0.f, dotProduct(pixelToLightVec, normal));

        diffuse = light.diffuseComponent * mat.diffuse * cosTeta;

        //specular
        Vec3 pixelToEyeVec = cameraLocation - location3D;
        // pixelToEyeVec.z -= 1;
        pixelToEyeVec.Normalize();
        Vec3 h = pixelToLightVec + pixelToEyeVec;
        h.Normalize();
        float cosAlpha = dotProduct(normal.getNormalized(), h);

        if (cosAlpha < 0)
            cosAlpha = 0;

        specular = light.specularComponent * mat.specular * std::pow(cosAlpha, mat.brillance);

        total += ambient + diffuse + specular;
    }

    return total;
}