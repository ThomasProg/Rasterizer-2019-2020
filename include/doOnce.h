#ifndef _DO_ONCE_H_
#define _DO_ONCE_H_

#include <functional>

struct DoOnce
{
private:
    bool lastState = false;
    
public:
    bool isOn = false;
    std::function<void(bool isOn)> onSwitch = [](bool){};
    
    void input(bool b);
};

#endif
