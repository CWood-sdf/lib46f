#include "DriveController.h"

double DriveController::getPctSpeed(const controller::axis& joystick)
{
    double joystickValue = abs(joystick.value()) > threshold ? joystick.value() : 0;
    joystickValue /= 1.27;
    return joystickValue;
}
DriveController::DriveController(Chassis* chassis)
{
    this->chassis = chassis;
}
DriveController::DriveController(Chassis* chassis, int threshold) : DriveController(chassis)
{
    this->threshold = threshold;
}
DriveController::DriveController(Chassis* chassis, int threshold, std::function<double(double)> speedCurve) : DriveController(chassis, threshold)
{
    this->joystickSpeedCurve = speedCurve;
}
DriveController::DriveController(Chassis* chassis, std::function<double(double)> speedCurve) : DriveController(chassis)
{
    this->joystickSpeedCurve = speedCurve;
}
DriveController& DriveController::setSpeedCurve(std::function<double(double)> speedCurve)
{
    this->joystickSpeedCurve = speedCurve;
    CHAIN
}
DriveController& DriveController::setThreshold(int theshold)
{
    this->threshold = theshold;
    CHAIN
}
DriveController& DriveController::useQuadraticSpeedCurve()
{
    this->joystickSpeedCurve = [](double pct)
    {
        pct /= 10.0;
        pct *= pct * sign(pct);
        return pct;
    };
    CHAIN
}
DriveController& DriveController::driveArcade(const controller::axis& forwardAxis, const controller::axis& turnAxis, bool reverseForward, bool reverseTurn)
{
    chassis->setSpeedLimit(100);
    double forward = (reverseForward * -2 + 1) * joystickSpeedCurve(getPctSpeed(forwardAxis));
    double turn = (reverseTurn * -2 + 1) * joystickSpeedCurve(getPctSpeed(turnAxis));
    chassis->driveFromDiff(forward, turn);
    CHAIN
}
DriveController& DriveController::driveTank(const controller::axis& leftAxis, const controller::axis& rightAxis)
{
    chassis->setSpeedLimit(100);
    double left = joystickSpeedCurve(getPctSpeed(leftAxis));
    double right = joystickSpeedCurve(getPctSpeed(rightAxis));
    chassis->driveFromLR(left, right);
    CHAIN
}
