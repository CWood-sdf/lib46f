#include "EPA_Tracker.h"
TrackingWheel::TrackingWheel(bool reverse, double wheelDiameter)
{
    this->reverse = reverse;
    this->wheelDiameter = wheelDiameter;
}
TrackingWheel::TrackingWheel(int32_t port, bool reverse, double wheelDiameter) : TrackingWheel(reverse, wheelDiameter)
{
    rotation* rot = new rotation(port);
    encoder = new Encoder(rot);
    encoder->resetPosition();
    this->rot = rot;
}
TrackingWheel::TrackingWheel(vex::triport::port port, bool reverse, double wheelDiameter) : TrackingWheel(reverse, wheelDiameter)
{
    class encoder* rot = new class encoder(port);
    encoder = new Encoder(rot);
}
TrackingWheel::TrackingWheel(motor& m, bool reverse, double gearRatio, double wheelDiameter) : TrackingWheel(reverse, wheelDiameter / gearRatio)
{
    encoder = new Encoder(&m);
}
TrackingWheel::TrackingWheel(Encoder& encoder, bool reverse, double wheelDiameter) : TrackingWheel(reverse, wheelDiameter)
{
    this->encoder = &encoder;
}
void Inertial::update()
{

    lastAngle = currentAngle;
    currentAngle = sensor->orientation(orientationType::yaw, rotationUnits::deg) + offset;
    // cout << angle << endl;
    double gain = posNeg180(currentAngle - lastAngle);
    gain *= (gain > 0 ? errPos : errNeg);
    currentAngle = lastAngle + gain;
}
Inertial::Inertial(int32_t port, double fullTurnNeg, double fullTurnPos)
{
    sensor = new inertial(port);
    AddDevice("Inertial", sensor);
    errNeg = 360.0 / fullTurnNeg;
    errPos = 360.0 / fullTurnPos;
    update();
}
Inertial::Inertial(inertial& sensor, double fullTurnNeg, double fullTurnPos)
{
    this->sensor = &sensor;
    errNeg = 360.0 / fullTurnNeg;
    errPos = 360.0 / fullTurnPos;
    update();
}
double Inertial::heading()
{
    return currentAngle;
}
double Inertial::avgHeading()
{
    return (currentAngle + lastAngle) / 2.0;
}
double Inertial::lastHeading()
{
    return lastAngle;
}
double Inertial::deltaAngle()
{
    return posNeg180(currentAngle - lastAngle);
}
Positioner::Positioner(encoderArr xArr, encoderArr yArr, Inertial s, PVector fromCenter) : angleSensor(s)
{
    this->fromCenter = fromCenter;
    this->xEncoders = xArr;
    this->yEncoders = yArr;
    for (int i = 0; i < xEncoders.size(); i++)
    {
        if (xEncoders[i].rot != NULL)
        {
            AddDevice("Odom X " + toCcp(i), xEncoders[i].rot);
        }
        lastX.push_back(xEncoders[i]->position(rotationUnits::deg));
    }
    for (int i = 0; i < yEncoders.size(); i++)
    {
        if (yEncoders[i].rot != NULL)
        {
            AddDevice("Odom Y " + toCcp(i), yEncoders[i].rot);
        }
        lastY.push_back(yEncoders[i]->position(rotationUnits::deg));
    }
    encXAmnt = xEncoders.size();
    encYAmnt = yEncoders.size();
}
void Positioner::setPos(PVector pos, double a)
{
    this->pos = pos;
    angleSensor.setAngle(a);
    PVector newP = position();
    PVector dir = newP - pos;
    this->pos = pos + dir;
}
// Shifts angle to range of [0, 360)
double baseAngle(double ang)
{
    while (ang >= 360.0)
    {
        ang -= 360.0;
    }
    while (ang < 0.0)
    {
        ang += 360.0;
    }
    return ang;
}
// Shifts an angle to a range of (-180, 180]
double posNeg180(double ang)
{
    double ret = baseAngle(ang);
    while (ret > 180.0)
    {
        ret -= 360.0;
    }
    while (ret <= -180.0)
    {
        ret += 360.0;
    }
    return ret;
}
#ifndef WINDOWS
// Function that updates the position
// 80+ lines of trig, vector math, and some sensor stuff
PVector Positioner::update()
{

    angleSensor.update();
    // Vector of the wheel angles
    PVector angles = PVector();

    // Get encoder rotation as fast as possible
    // Use raw array for speed
    vector<double> rotX = {};
    vector<double> rotY = {};
    // Update bot angle as close to rotation access as possible
    //  updateBotAngle(run);
    if (xEncoders.size() != 0)
    {
        for (int i = 0; i < xEncoders.size(); i++)
        {
            double position = xEncoders[i]->position(rotationUnits::deg);
            rotX.push_back(position);
        }
    }
    // cout << yEncoders.size() << endl;
    if (yEncoders.size() != 0)
    {
        for (int i = 0; i < yEncoders.size(); i++)
        {
            rotY.push_back(yEncoders[i]->position(rotationUnits::deg));
        }
    }

    for (int i = 0; i < xEncoders.size(); i++)
    {
        // Get the rotation in radians
        double rot = posNeg180(rotX[i] - lastX[i]) * DEG_TO_RAD * xEncoders[i].mult() * xEncoders[i].wheelRadius();
        // add the change in rotation to angles.x
        angles.x += rot;
        // Reset the last rotation
        lastX[i] = rotX[i];
    }

    // Same thing here
    for (int i = 0; i < yEncoders.size(); i++)
    {
        double rot = posNeg180(rotY[i] - lastY[i]) * DEG_TO_RAD * yEncoders[i].mult() * yEncoders[i].wheelRadius();

        angles.y += rot;
        lastY[i] = rotY[i];
    }

    // Average the angles
    if (yEncoders.size() != 0)
    {
        angles.y /= encYAmnt;
    }
    if (xEncoders.size() != 0)
    {
        angles.x /= encXAmnt;
    }
    PVector deltaAngles;

    if (angleSensor.deltaAngle() != 0.0)
    {
        double deltaAngle = angleSensor.deltaAngle() * DEG_TO_RAD;
        double sin2 = 2.0 * sin(deltaAngle / 2.0);
        double x = (angles.x / deltaAngle) * sin2;
        double y = (angles.y / deltaAngle) * sin2;
        deltaAngles = {x, y};
        deltaAngles.rotate(angleSensor.avgHeading());
    }
    else
    {
        deltaAngles = angles;
        deltaAngles.rotate(angleSensor.avgHeading());
    }

    // Get the change in the position
    PVector deltaPos = deltaAngles;

    speed = deltaPos.dist2D() / (time.time(seconds));
    time.reset();
    pos += deltaPos; // Add deltaPos to pos
    s(V5_SENSOR_REFRESH);
    return pos; // Return pos so it can be used
}
PVector Positioner::position()
{
    PVector fromCenterCopy = fromCenter;
    fromCenterCopy.rotate(angleSensor.heading());
    PVector posCopy = pos;
    posCopy *= -1.0;
    posCopy += fromCenterCopy;
    return posCopy;
}
double Positioner::xPosition(distanceUnits)
{
    return position().x;
}
double Positioner::yPosition(distanceUnits)
{
    return position().y;
}
double Positioner::heading()
{
    return angleSensor.heading();
}
FieldCoord Positioner::fullPos()
{
    return FieldCoord(position(), heading());
}
bool Positioner::moving()
{
    // If the velocity is greater than 0.01 in/s or
    // its been less a second since the last call to clearMove
    return abs(speed) > 0.01 ||
           time.time(timeUnits::sec) < 1;
}
double Positioner::velocity()
{
    return speed;
}
void Positioner::clearMove()
{
    time.clear();
}
#endif
