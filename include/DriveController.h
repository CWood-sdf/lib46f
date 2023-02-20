#include "Odometry/Controllers.h"

class DriveController
{
    Chassis* chassis;
    std::function<double(double)> joystickSpeedCurve = [](double v)
    {
        return v;
    };
    int threshold = 20;
    double getPctSpeed(const controller::axis& joystick);

public:
    DriveController(Chassis* chassis);
    DriveController(Chassis* chassis, int threshold);
    DriveController(Chassis* chassis, int threshold, std::function<double(double)> speedCurve);
    DriveController(Chassis* chassis, std::function<double(double)> speedCurve);
    DriveController& setSpeedCurve(std::function<double(double)> speedCurve);
    DriveController& setThreshold(int theshold);
    DriveController& useQuadraticSpeedCurve();
    DriveController& driveArcade(const controller::axis& forwardAxis, const controller::axis& turnAxis, bool reverseForward = false, bool reverseTurn = false);
    DriveController& driveTank(const controller::axis& leftAxis, const controller::axis& rightAxis);
};
