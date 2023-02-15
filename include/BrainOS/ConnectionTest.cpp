#include "BrainOS/ConnectionTest.h"
extern vector<tuple<string, vex::device*, bool, bool>> connectedDevices;
void testConnection(bool)
{
    int i = 0;
    int j = 0;
    Brain.Screen.clearScreen(black);
    for (auto device : connectedDevices)
    {
        string name = get<0>(device);
        vex::device* dev = get<1>(device);
        bool isMotor = get<2>(device);
        if (dev->installed())
        {
            if (isMotor && ((motor*)dev)->temperature() > 50)
            {
                // hue 35 is orange
                Brain.Screen.setPenColor(35);
                Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
            }
            else
            {
                Brain.Screen.setPenColor(vex::color::green);
                Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
            }
        }
        else
        {
            Brain.Screen.setPenColor(vex::color::red);
            Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
        }
        if (i * 19 + 20 > BRAIN_HEIGHT - 20)
        {
            i = 0;
            j++;
        }
        else
        {
            i++;
        }
    }
    Brain.Screen.setPenColor(vex::color::white);
}
