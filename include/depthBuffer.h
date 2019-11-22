#ifndef _DEPTH_BUFFER_H_
#define _DEPTH_BUFFER_H_

class DepthBuffer
{
public:
    const unsigned int width, height;
public:
    float* depth = nullptr;

public:
    DepthBuffer() = delete;
    DepthBuffer(unsigned int width, unsigned int height);
    ~DepthBuffer();

    void resetDepth();
    void setDepth(unsigned int x, unsigned int y, float newDepth);
    float getDepth(unsigned int x, unsigned int y) const;
};

#endif