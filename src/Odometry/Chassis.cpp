#include "Chassis.h"
#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.0174532925199432957692369076842
#endif
PVector Chassis::botPos()
{
    return pos.position();
}
double Chassis::botAngle()
{
    return pos.heading();
}
Chassis::Chassis(MotorGroup& left, MotorGroup& right, Positioner& p, double trackWidth, double gearRatio, double wheelDiameter, gearSetting cartridge) : leftWheels(left), rightWheels(right), pos(p)
{

    this->trackWidth = trackWidth;
    this->gearRatio = gearRatio;
    this->wheelRad = wheelDiameter / 2.0;
}

void Chassis::turnRight(double speed)
{
    driveFromDiff(0, speed);
}
void Chassis::turnLeft(double speed)
{
    driveFromDiff(0, -speed);
}
void Chassis::moveRightSide(double speed)
{
    rightWheels.spinVolt(speed > 0 ? fwd : vex::reverse, abs(speed));
}
void Chassis::moveLeftSide(double speed)
{
    leftWheels.spinVolt(speed > 0 ? fwd : vex::reverse, abs(speed));
}
void Chassis::holdBrake()
{
    leftWheels.stop(hold);
    rightWheels.stop(hold);
}
void Chassis::coastBrake()
{
    leftWheels.stop(coast);
    rightWheels.stop(coast);
}
Chassis::chain_method Chassis::setSpeedLimit(double v)
{
    speedLimit = v;
    CHAIN
}
double sign(double);
void Chassis::driveFromDiff(double speed, double diff)
{
    if (abs(speed) > speedLimit)
    {
        speed = speedLimit * sign(speed);
    }
    double left = speed + diff;
    double right = speed - diff;
    leftWheels.spinVolt(left < 0 ? reverse : fwd, abs(left));
    rightWheels.spinVolt(right < 0 ? reverse : fwd, abs(right));
}
// Speed is in pct, velocity is in rad/sec
void Chassis::driveFromAngular(double speed, double vel)
{
    driveFromCurvature(speed, 1 / pctToReal(speed) * vel);
}
// Speed is in pct, curvature is 1/in
void Chassis::driveFromCurvature(double speed, double curvature)
{
    double targetRobotVel = pctToReal(speed);
    double rightExtra = realToPct(curvature * (getTrackWidth() + 1.0) * targetRobotVel / 2.0);
    while (abs(rightExtra) + abs(speed) > 100.0)
    {
        speed = (abs(speed) - 1.0) * sign(speed);
        targetRobotVel = pctToReal(speed);
        rightExtra = realToPct(curvature * (getTrackWidth() + 1.0) * targetRobotVel / 2.0);
        if (abs(speed) < 1.0)
            break;
    }
    driveFromDiff(speed, rightExtra);
}
void Chassis::driveFromLR(double left, double right)
{
    double diff = (left - right) / 2.0;
    driveFromDiff((left + right) / 2.0, diff);
}
