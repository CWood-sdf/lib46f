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
Chassis::Chassis(MotorGroup& left, MotorGroup& right, Positioner& p, double trackWidth, double gearRatio, double wheelRad, gearSetting cartridge) : leftWheels(left), rightWheels(right), pos(p)
{

    this->trackWidth = trackWidth;
    this->gearRatio = gearRatio;
    this->wheelRad = wheelRad;
}

void Chassis::turnRight(double speed)
{
    driveFromDiff(0, speed);
}
void Chassis::turnLeft(double speed)
{
    driveFromDiff(0, -speed);
}
void Chassis::moveRightSide(double speed, directionType d)
{
    rightWheels.spinVolt(d, speed);
}
void Chassis::moveLeftSide(double speed, directionType d)
{
    leftWheels.spinVolt(d, speed);
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
Chassis::chain_method Chassis::setMaxAcc(double v)
{
    maxAcc = v;
    CHAIN
}
Chassis::chain_method Chassis::setMaxDAcc(double v)
{
    maxDAcc = v;
    CHAIN
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
    double realSpeed, leftSpeed, rightSpeed;
    int count = 0;
TOP:
    realSpeed = pctToReal(speed);
    leftSpeed = realToPct(realSpeed * (2.0 + curvature * trackWidth) / 2.0);
    rightSpeed = realToPct(realSpeed * (2.0 - curvature * trackWidth) / 2.0);
    if ((abs(leftSpeed) > 100 || abs(rightSpeed) > 100) && realToPct(abs(curvature * trackWidth)) < 100 && count++ < 5)
    {
        double max = std::max(leftSpeed, rightSpeed);
        if (max < 100)
        {
            max = std::min(leftSpeed, rightSpeed);
        }
        speed *= 100 / max;
        goto TOP;
    }
    driveFromLR(leftSpeed, rightSpeed);
}
void Chassis::driveFromLR(double left, double right)
{
    double diff = (left - right) / 2.0;
    driveFromDiff((left + right) / 2.0, diff);
}
