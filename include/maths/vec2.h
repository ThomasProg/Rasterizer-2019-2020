#ifndef _VEC2_H_
#define _VEC2_H_

class Vec3;

struct Vec2
{
public:
    float x, y;

public:
    Vec2();
    Vec2(float x, float y);
    Vec2(const Vec2&);
    Vec2(const Vec3&);

    float GetMagnitude() const;

    void toUnit();
};

#endif