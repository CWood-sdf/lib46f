#ifndef EPA_TRACKER_H
#define EPA_TRACKER_H
#include "FieldCoord.h"
#include "Sensors/Wrappers/Encoder.h"
// EPA_Tracker.h -- Use this file to track the robot's absolute position on the field
// This file does so much math that it be like Beethoven 9 if it all works properly

// Make some basic conversion numbers
// To use, just multiply the number by the conversion
#define CM_TO_IN (1.0 / 2.54)
#define IN_TO_CM 2.54
#define DEG_TO_RAD (M_PI / 180.0)
#define RAD_TO_DEG (1.0 / DEG_TO_RAD)
typedef unsigned int uint;

double baseAngle(double);
double posNeg180(double);
void updateBotAngle(bool add = false);
// A class that stores a reference of something as a pointer
//   this is used so that I can put references in arrays
template <class tp>
struct Ref
{
    tp* val;
    Ref() {}
    Ref(tp& v)
    {
        val = &v;
    }
    tp& operator*()
    {
        return *val;
    }
    operator tp&()
    {
        return *val;
    }
    operator tp*()
    {
        return val;
    }
    tp* operator->()
    {
        return val;
    }
};
// //Use the template command for the amount of encoders used
// //Each encoder is used for each dimension
// //Positioner<2, 2> has four encoders total 2x, 2y
// template<uint encodersX, uint encodersY>
// class Port
// {
//     const int32_t port;

// public:
//     Port(int32_t p) : port(p) {}
//     int32_t getPort()
//     {
//         return port;
//     }
// };

#ifndef WINDOWS
struct AddDevice
{
    AddDevice(string name, vex::device* device);
    AddDevice(string name, vex::motor* deivce);
    AddDevice(string name, vex::motor* device, bool drive);
};
class Positioner;
class TrackingWheel
{
    friend class Positioner;
    Encoder* encoder;
    bool reverse;
    double wheelDiameter;
    TrackingWheel(bool reverse, double wheelDiameter);
    rotation* rot = NULL;

public:
    TrackingWheel(int32_t port, bool reverse, double wheelDiameter);
    TrackingWheel(vex::triport::port port, bool reverset, double wheelDiameter);
    TrackingWheel(motor& m, bool reverse, double gearRatio, double wheelDiameter);
    TrackingWheel(Encoder& e, bool reverse, double wheelDiameter);
    Encoder* operator->()
    {
        return encoder;
    }
    double mult()
    {
        return reverse ? -1.0 : 1.0;
    }
    double wheelRadius()
    {
        return wheelDiameter / 2.0;
    }
};
class Inertial
{
    friend class Positioner;
    inertial* sensor;
    double errNeg = 1.0;
    double errPos = 1.0;
    double lastAngle = 0.0;
    double currentAngle = 0.0;
    double offset = 0.0;
    void update();

public:
    Inertial(int32_t port, double fullTurnNeg, double fullTurnPos);
    Inertial(inertial& sensor, double fullTurnNeg, double fullTurnPos);
    double heading();
    double avgHeading();
    double lastHeading();
    double deltaAngle();
    void init()
    {
        gyroInit(*sensor);
    }
    void setAngle(double a)
    {
        offset += a - heading();
        lastAngle = currentAngle = a;
    }
};
class Positioner
{
    // A few typedefs
public:
    typedef vector<TrackingWheel> encoderArr;

    double speed = 0.0;
    // const double size = encodersX + encodersY;
    // Private variables
private:
    double encXAmnt;
    double encYAmnt;
    Inertial angleSensor;
    typedef vector<double> doubleArr;
    doubleArr lastX;
    doubleArr lastY;
    PVector fromCenter = PVector(0.0, 0.0);

    timer time;

    encoderArr xEncoders; // Make the x encoder array
    encoderArr yEncoders;

private:
    PVector pos = PVector(0.0, 0.0); // Make a vector to store the current position

public:
    void setPos(PVector pos, double a);
    // The constructors

    Positioner(encoderArr encodersX, encoderArr encodersY, Inertial angler, PVector fromCenter = PVector(0.0, 0.0));
    Positioner() = delete;
    // Function that updates the position
    // 80+ lines of trig, vector math, and some sensor stuff
    PVector update();
    PVector position();
    double xPosition(distanceUnits = inches);
    double yPosition(distanceUnits = inches);
    double heading();
    FieldCoord fullPos();
    bool moving();
    double velocity();
    void clearMove();
    void init()
    {
        angleSensor.init();
    }
};
#endif
#endif
