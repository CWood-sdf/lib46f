#include "BrainOS/ConnectionTest.h"
extern vector<tuple<string, vex::device*, bool, bool>> connectedDevices;
void testConnection(bool)
{
    int i = 0;
    int j = 0;
    static color orange = vex::color(255, 165, 0);
    vexDisplayBackgroundColor(black);
    // Brain.Screen.clearScreen(black);
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
                vexDisplayForegroundColor(orange);
                // Brain.Screen.setPenColor(35);
                vexDisplayPrintf(10 + j * 180, 20 + i * 19, 0, "%s-%d", name.c_str(), dev->index() + 1);
                // Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
            }
            else
            {
                vexDisplayForegroundColor(green);
                // Brain.Screen.setPenColor(vex::color::green);
                vexDisplayPrintf(10 + j * 180, 20 + i * 19, 0, "%s-%d", name.c_str(), dev->index() + 1);
                // Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
            }
        }
        else
        {
            vexDisplayForegroundColor(red);
            // Brain.Screen.setPenColor(vex::color::red);
            vexDisplayPrintf(10 + j * 180, 20 + i * 19, 0, "%s-%d", name.c_str(), dev->index() + 1);
            // Brain.Screen.printAt(10 + j * 180, 20 + i * 19, true, "%s-%d", name.c_str(), dev->index() + 1);
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
    vexDisplayForegroundColor(vex::color::white);
    // Brain.Screen.setPenColor(vex::color::white);
}
