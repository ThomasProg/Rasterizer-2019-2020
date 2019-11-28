#include "doOnce.h"

void DoOnce::input(bool b)
{
    if (b && lastState != b)
    {
        isOn = !isOn;
        onSwitch(isOn);
    }
    lastState = b;
}