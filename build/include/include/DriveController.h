#include "Odometry/Controllers.h"

class DriveController {
    Chassis* chassis;
    std::function<double(double)> joystickSpeedCurve = [](double v) {
        return v;
    };
    int threshold = 20;
    double getPctSpeed(const controller::axis& joystick);

public:
    /**
     * @brief Construct a new Drive Controller object
     *
     * @param chassis The chassis to drive the motors on
     */
    DriveController(Chassis* chassis);
    /**
     * @brief Construct a new Drive Controller object
     *
     * @param chassis The chassis to drive the motors on
     * @param threshold The threshold for the joystick to be considered pressed
     */
    DriveController(Chassis* chassis, int threshold);

    /**
     * @brief Construct a new Drive Controller object
     *
     * @param chassis The chassis to drive the motors on
     * @param threshold The threshold for the joystick to be considered pressed
     * @param speedCurve The speed curve to use for the joystick
     *
     */
    DriveController(Chassis* chassis, int threshold, std::function<double(double)> speedCurve);
    /**
     * @brief Construct a new Drive Controller object
     *
     * @param chassis The chassis to drive the motors on
     * @param speedCurve The speed curve to use for the joystick
     */
    DriveController(Chassis* chassis, std::function<double(double)> speedCurve);
    /**
     * @brief Set the Speed Curve for the controller
     *
     * @param speedCurve The speed curve
     * @return DriveController&
     */
    DriveController& setSpeedCurve(std::function<double(double)> speedCurve);
    /**
     * @brief Set the joystick threshold
     *
     * @param theshold The threshold
     * @return DriveController&
     */
    DriveController& setThreshold(int theshold);
    /**
     * @brief Use a quadratic speed curve
     *
     * @return DriveController&
     */
    DriveController& useQuadraticSpeedCurve();
    /**
     * @brief Drive the robot in an arcade style (forward and turn)
     *
     * @param forwardAxis The forward axis
     * @param turnAxis The turn axis
     * @param reverseForward Whether to reverse the forward axis
     * @param reverseTurn Whether to reverse the turn axis
     * @return DriveController&
     */
    DriveController& driveArcade(const controller::axis& forwardAxis, const controller::axis& turnAxis, bool reverseForward = false, bool reverseTurn = false);
    /**
     * @brief Drive the robot in a tank style (left and right)
     *
     * @param leftAxis The left axis
     * @param rightAxis The right axis
     * @return DriveController&
     */
    DriveController& driveTank(const controller::axis& leftAxis, const controller::axis& rightAxis);
};
