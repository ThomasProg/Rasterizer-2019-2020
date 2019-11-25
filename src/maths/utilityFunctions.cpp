#include "utilityFunctions.h"

float clamp(float value, float low, float high)
{
    if (value > high)
        return high;
    else if (value < low)
        return low;
    else 
        return value;
}