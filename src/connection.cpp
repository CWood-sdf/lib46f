#include "connection.h"
std::vector<tuple<string, vex::device*, bool, bool>> connectedDevices = {};

AddDevice::AddDevice(string name, vex::device* device)
{
    connectedDevices.push_back(make_tuple(name, device, false, false));
}
AddDevice::AddDevice(string name, vex::motor* device)
{
    connectedDevices.push_back(make_tuple(name, device, true, false));
}
AddDevice::AddDevice(string name, vex::motor* device, bool drive)
{
    connectedDevices.push_back(make_tuple(name, device, true, drive));
}
#define TestDevice(device) AddDevice device##AddDevice(#device, &device);
#define TestDriveMotor(device) AddDevice device##AddDevice(#device, &device, true);
// Make a brain

#define TEST_MOT(m, n) \
    cout << n << endl; \
    m->spin(fwd);      \
    s(1000);           \
    m->stop();         \
    s(1000);
void testDriveConfiguration()
{
    for (auto i : connectedDevices)
    {
        if (get<3>(i))
        {
            TEST_MOT(((motor*)get<1>(i)), get<0>(i))
        }
    }
}
#define TMC(m)                                                  \
    if (!m.installed())                                         \
    {                                                           \
        cout << "Motor " << #m << " is not connected!" << endl; \
        Greg.rumble(".");                                       \
    }
void testDeviceConnection()
{
    // TMC(FL)
    // TMC(ML)
    // TMC(BL)
    // TMC(FR)
    // TMC(MR)
    // TMC(BR)
    for (auto i : connectedDevices)
    {
        if (!get<1>(i)->installed())
        {
            int32_t port = get<1>(i)->index() + 1;
            cout << get<0>(i) << " is not connected on port " << port << "!" << endl;
        }
    }
}
