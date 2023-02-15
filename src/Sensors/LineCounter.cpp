#include "Sensors/LineCounter.h"

bool LineCounter::firstHit()
{
    if (fHit)
    {
        fHit = false;
        return true;
    }
    return false;
}
bool LineCounter::active()
{
    return isActive;
}
void LineCounter::update()
{

    bool isActive = pressing();
    if (wasActiveLast)
    {
        if (!isActive)
        {
            countOut++;
        }
    }
    else
    {
        if (isActive)
        {
            countIn++;
            fHit = true;
        }
    }
    wasActiveLast = isActive;
}
void LineCounter::reset()
{
    countIn = 0;
    countOut = 0;
    if (active())
    {
        countIn = 1;
        countOut = 0;
    }
}
int LineCounter::getCountOut()
{
    return countOut;
}
int LineCounter::getCountIn()
{
    return countIn;
}
bool LineCounter::pressing()
{

    isActive = threshold <= sensor->reflectivity();
    // cout << sensor->reflectivity() << endl;
    if (isActive)
    {
        threshold = lowThreshold;
    }
    else
    {
        threshold = startThreshold;
    }
    return isActive;
}
int LineCounter::rawData()
{
    return sensor->reflectivity();
}

const KillThread LineCounter::updater = thread([]()
    {
    if(LineCounter::instances.size() == 0){
        cout << "No Line Counter instances exist, exiting thread" << endl;
        return;
    }
    while(1){
        for(auto t : LineCounter::instances){
            t->update();
        }
        s(10);
  } });
