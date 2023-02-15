#include "robot-config.h"
#ifdef MAKE

void microWait(uint);
#else
void microWait(uint time)
{
    auto startTime = Brain.Timer.systemHighResolution();
    int i = 0;
    while (Brain.Timer.systemHighResolution() - startTime < time || i < -10)
    {
        i++;
    }
}

#endif
