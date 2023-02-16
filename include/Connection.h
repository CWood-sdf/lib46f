// #include "EPA_Tracker.h"
#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H
#include "Sensors/LineCounter.h"
#include "Sensors/PotDial.h"
// #include "Sensors/Distance.h"
#include "Odometry/Controllers.h"
#include "Sensors/Wrappers/Encoder.h"

extern std::vector<tuple<string, vex::device*, bool, bool>> connectedDevices;
#define TestDevice(device) AddDevice device##AddDevice(#device, &device);
#define TestDriveMotor(device) AddDevice device##AddDevice(#device, &device, true);

void testDriveConfiguration();

void testDeviceConnection();
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
#endif
