#ifndef _DO_ONCE_H_
#define _DO_ONCE_H_

#include <functional>

struct DoOnce
{
private:
    bool lastState = false;
    
public:
    bool isOn = false;
    std::function<void()> onSwitch = [](){};
    
    void input(bool b);
};

#endif
