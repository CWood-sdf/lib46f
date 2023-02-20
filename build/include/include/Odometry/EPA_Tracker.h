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
struct Ref {
    tp* val;
    Ref() {}
    Ref(tp& v) {
        val = &v;
    }
    tp& operator*() {
        return *val;
    }
    operator tp&() {
        return *val;
    }
    operator tp*() {
        return val;
    }
    tp* operator->() {
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
struct AddDevice {
    AddDevice(string name, vex::device* device);
    AddDevice(string name, vex::motor* deivce);
    AddDevice(string name, vex::motor* device, bool drive);
};
class Positioner;
class TrackingWheel {
    friend class Positioner;
    Encoder* encoder;
    bool reverse;
    double wheelDiameter;
    TrackingWheel(bool reverse, double wheelDiameter);
    rotation* rot = NULL;
    Encoder* operator->() {
        return encoder;
    }
    double mult() {
        return reverse ? -1.0 : 1.0;
    }
    double wheelRadius() {
        return wheelDiameter / 2.0;
    }

public:
    /**
     * @brief Construct a new Tracking Wheel object with a rotation sensor
     *
     * @param port the port of the rotation sensor
     * @param reverse Whether to reverse the wheel's direction
     * @param wheelDiameter The tracking wheel diameter
     */
    TrackingWheel(int32_t port, bool reverse, double wheelDiameter);
    /**
     * @brief Construct a new Tracking Wheel object with a shaft encoder
     *
     * @param port The port of the shaft encoder
     * @param reverse Whether to reverse the wheel's direction
     * @param wheelDiameter The tracking wheel diameter
     */
    TrackingWheel(vex::triport::port& port, bool reverse, double wheelDiameter);
    /**
     * @brief Construct a new Tracking Wheel object with a motor
     *
     * @param m The motor to use
     * @param reverse Whether to reverse the wheel's direction
     * @param gearRatio
     * @param wheelDiameter
     */
    TrackingWheel(motor& m, bool reverse, double gearRatio, double wheelDiameter);
    /**
     * @brief Construct a new Tracking Wheel object with an encoder
     *
     * @param e The encoder to use
     * @param reverse Whether to reverse the wheel's direction
     * @param wheelDiameter The tracking wheel diameter
     */
    TrackingWheel(Encoder& e, bool reverse, double wheelDiameter);
};
class Inertial {
    friend class Positioner;
    inertial* sensor;
    double errNeg = 1.0;
    double errPos = 1.0;
    double lastAngle = 0.0;
    double currentAngle = 0.0;
    double offset = 0.0;
    void update();

public:
    /**
     * @brief Construct a new Inertial object
     *
     * @param port The inertial sensor's port
     * @param fullTurnNeg The value that the sensor returns when it is turned 360 degrees in the negative direction (ie from 360 to 0 or counter-clockwise)
     * @param fullTurnPos The value that the sensor returns when it is turned 360 degrees in the positive direction (ie from 0 to 360 or clockwise)
     */
    Inertial(int32_t port, double fullTurnNeg, double fullTurnPos);
    /**
     * @brief Construct a new Inertial object
     *
     * @param sensor The inertial sensor
     * @param fullTurnNeg The value that the sensor returns when it is turned 360 degrees in the negative direction (ie from 360 to 0 or counter-clockwise)
     * @param fullTurnPos The value that the sensor returns when it is turned 360 degrees in the positive direction (ie from 0 to 360 or clockwise)
     */
    Inertial(inertial& sensor, double fullTurnNeg, double fullTurnPos);
    /**
     * @brief Get the current heading of the robot
     *
     * @return double
     */
    double heading();
    /**
     * @brief Get the average value between the current and last update
     *
     * @return double
     */
    double avgDeltaHeading();
    /**
     * @brief Get the last heading of the robot
     *
     * @return double
     */
    double lastHeading();
    /**
     * @brief Get the change in heading since the last update
     *
     * @return double
     */
    double deltaHeading();
    /**
     * @brief Calibrate the sensor
     *
     */
    void init() {
        gyroInit(*sensor);
    }
    /**
     * @brief Set the angle
     *
     * @param a
     */
    void setHeading(double a) {
        offset += a - heading();
        lastAngle = currentAngle = a;
    }
};
class Positioner {
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
    void init() {
        angleSensor.init();
    }
};
#endif
#endif
