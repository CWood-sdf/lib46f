#include "Odometry/EPA_Wheel_Control.h"
double sign(double);
class DriveController
{
    Chassis *chassis;
    std::function<double(double)> joystickSpeedCurve = [](double v)
    {
        return v;
    };
    int threshold = 20;
    double getPctSpeed(controller::axis &joystick)
    {
        double joystickValue = abs(joystick.value()) > threshold ? joystick.value() : 0;
        joystickValue /= 1.27;
        return joystickValue;
    }

public:
    DriveController(Chassis *chassis)
    {
        this->chassis = chassis;
    }
    DriveController(Chassis *chassis, int threshold) : DriveController(chassis)
    {
        this->threshold = threshold;
    }
    DriveController(Chassis *chassis, int threshold, std::function<double(double)> speedCurve) : DriveController(chassis, threshold)
    {
        this->joystickSpeedCurve = speedCurve;
    }
    DriveController(Chassis *chassis, std::function<double(double)> speedCurve) : DriveController(chassis)
    {
        this->joystickSpeedCurve = speedCurve;
    }
    DriveController &setSpeedCurve(std::function<double(double)> speedCurve)
    {
        this->joystickSpeedCurve = speedCurve;
        CHAIN
    }
    DriveController &setThreshold(int theshold)
    {
        this->threshold = theshold;
        CHAIN
    }
    DriveController &useQuadraticSpeedCurve()
    {
        this->joystickSpeedCurve = [](double pct)
        {
            pct /= 10.0;
            pct *= pct * sign(pct);
            return pct;
        };
        CHAIN
    }
    DriveController &driveArcade(controller::axis &forwardAxis, controller::axis &turnAxis, bool reverseForward = false, bool reverseTurn = false)
    {
        chassis->setSpeedLimit(100);
        double forward = (reverseForward * -2 + 1) * joystickSpeedCurve(getPctSpeed(forwardAxis));
        double turn = (reverseTurn * -2 + 1) * joystickSpeedCurve(getPctSpeed(turnAxis));
        chassis->driveFromDiff(forward, turn);
        CHAIN
    }
    DriveController &driveTank(controller::axis &leftAxis, controller::axis &rightAxis)
    {
        chassis->setSpeedLimit(100);
        double left = joystickSpeedCurve(getPctSpeed(leftAxis));
        double right = joystickSpeedCurve(getPctSpeed(rightAxis));
        chassis->driveFromLR(left, right);
        CHAIN
    }
};
