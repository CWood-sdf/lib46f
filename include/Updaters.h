#include "ClassBaseFns.h"
#ifdef MAKE

void microWait(uint);
void executeThreads();
void updatePos();
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

void executeThreads()
{

    // Execute autonomous functions
    while (1)
    {
        if (threadFns.size() > 0)
        {
            threadFns.front()();
            threadFns.pop_front();
        }
        else
        {
            this_thread::sleep_for(10);
        }
    }
}
void updatePos()
{
    while (1)
    {
        positioner.update();
    }
}
#endif
