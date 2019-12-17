#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include "texture.h"
#include "depthBuffer.h"

struct Color;

struct FrameBuffer
{
public:
    const unsigned int width, height;
    Texture texture;
    DepthBuffer depthBuffer;

public:
    FrameBuffer() = delete;
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer();

    __inline
    void SetPixel(unsigned int x, unsigned int y, float newDepth, const Color& c);

    __inline
    Color GetPixelColor(unsigned int x, unsigned int y) const;
    __inline
    float GetDepth(unsigned int x, unsigned int y) const;

    void ResetPixels();
};

#include "frameBuffer.inl"

#endif