#ifndef CHASSIS_H
#define CHASSIS_H
#include "Bezier.h"
#include "EPA_Tracker.h"
#include "PID.h"

struct Chassis;
class SpeedController;
class Path
{
public:
    struct El
    {
        PVector bezierPt;
        double targetSpeed;
        double targetAngle;
        double curvature;
        operator PVector()
        {
            return bezierPt;
        }
    };

private:
    double maxAcc = 100; // in/s^2
    double maxDAcc = 80; // in/s^2
    typedef Path& chain_method;
    vector<El> path;
    VectorArr arr;
    double kConst;

public:
    chain_method setK(double s);
    void make(VectorArr& arr, Chassis* chassis);
    void remake(Chassis* chassis);
    VectorArr getBezier();
    int size();
    El& last();
    El& operator[](int index);
    /**
     * @brief Returns the maximum accelleration
     *
     * @return double
     */
    double getMaxAcc()
    {
        return maxAcc;
    }
    /**
     * @brief Returns the maximum deaccelleration
     *
     * @return double
     */
    double getMaxDAcc()
    {
        return maxDAcc;
    }
    /**
     * @brief Sets the max accelleration
     *
     * @param v The new max acc value
     * @return chain_method
     */
    chain_method setMaxAcc(double v);
    /**
     * @brief Set the Max decelleration
     *
     * @param v The new max deacc value
     * @return chain_method
     */
    chain_method setMaxDAcc(double v);
    /**
     * @brief Set the speed limit
     *
     * @param v The new speed limit value
     * @return chain_method
     */
};
struct Chassis
{
private:
    typedef Chassis& chain_method;
    double speedLimit = 100;
    MotorGroup& leftWheels;
    MotorGroup& rightWheels;
    double trackWidth = 0.0;
    double gearRatio = 1.0;
    double wheelRad = 0.0;
    gearSetting cartridge = gearSetting::ratio18_1;

public:
    /**
     * @brief The odometry tracker
     *
     */
    Positioner& pos;
    /**
     * @brief Get the Track Width
     *
     * @return double
     */
    double getTrackWidth()
    {
        return trackWidth;
    }
    /**
     * @brief Get the global angle of the robot
     *
     * @return double
     */
    double botAngle();
    /**
     * @brief Get the global position of the robot
     *
     * @return PVector
     */
    PVector botPos();
    /**
     * @brief Convert revolutions to inches
     *
     * @param rotation The rotations of the wheels
     * @return double The distance in inches
     */
    double revToInches(double rotation)
    {
        return rotation * 2.0 * M_PI * wheelRad / gearRatio;
    }
    /**
     * @brief Convert inches to revolutions
     *
     * @param inches The inches that the robot has moved
     * @return double The distance in revolutions
     */
    double inchesToRev(double inches)
    {
        return inches * gearRatio / 2.0 / M_PI / wheelRad;
    }
    /**
     * @brief Convert pct to in/s
     *
     * @param speedPct The speed in percent
     * @return double The speed in in/s
     */
    double pctToReal(double speedPct)
    {
        // rpm
        double motorVel;
        switch (cartridge)
        {
        case vex::gearSetting::ratio18_1:
            motorVel = 200;
            break;
        case vex::gearSetting::ratio36_1:
            motorVel = 100;
            break;
        case vex::gearSetting::ratio6_1:
            motorVel = 600;
            break;
        }
        return motorVel * speedPct / 100.0 / gearRatio * 2.0 * M_PI / 60.0 /*rad/s*/
               * wheelRad;
    }
    /**
     * @brief Convert in/s to pct
     *
     * @param speed The speed in in/s
     * @return double The speed in pct
     */
    double realToPct(double speed)
    {
        // rpm
        double motorVel;
        switch (cartridge)
        {
        case vex::gearSetting::ratio18_1:
            motorVel = 200;
            break;
        case vex::gearSetting::ratio36_1:
            motorVel = 100;
            break;
        case vex::gearSetting::ratio6_1:
            motorVel = 600;
            break;
        }
        return speed / motorVel * 100.0 * gearRatio / 2.0 / M_PI * 60.0 / wheelRad;
    }
    /**
     * @brief Drives the robot at the speed and curvature
     *
     * @param speedPct The speed to drive the robot in percent
     * @param curvature The curvature to drive the robot at in in^-1
     */
    void driveFromCurvature(double speedPct, double curvature);
    /**
     * @brief Drives the robot at the specified speed and angular velocity
     *
     * @param speedPct The speed in percent
     * @param angularVel The angular velocity in rad/s
     */
    void driveFromAngular(double speedPct, double angularVel);
    /**
     * @brief Drives the left and right sides at the specified speeds
     *
     * @param leftPct The speed of the left side in percent
     * @param rightPct
     */
    void driveFromLR(double leftPct, double rightPct);
    /**
     * @brief Drives the robot at the specified speeds
     *
     * @param speedPct The forward speed in percent
     * @param diffPct The speed difference in percent
     */
    void driveFromDiff(double speedPct, double diffPct);
    /**
     * @brief Hold brakes the drivetrain
     *
     */
    void holdBrake();
    /**
     * @brief Coast brakes the drivetrain
     *
     */
    void coastBrake();
    /**
     * @brief Turn left at the percent speed
     *
     * @param speedPct The speed to move in percent
     */
    void turnLeft(double speedPct);
    /**
     * @brief Turn right at the speed
     *
     * @param speedPct The speed to move in percent
     */
    void turnRight(double speedPct);
    /**
     * @brief Moves the left side at the given speed and direction
     *
     * @param speed The speed to move in percent
     */
    void moveLeftSide(double speedPct);
    /**
     * @brief Moves the right side at the given speed and direction
     *
     * @param speed The speed to move in percent
     */
    void moveRightSide(double speedPct);
    /**
     * @brief Returns the speed limit
     *
     * @return double
     */
    double getSpeedLimit()
    {
        return speedLimit;
    }
    /**
     * @brief Set the Speed Limit
     *
     * @param v The speed limit
     * @return chain_method
     */
    chain_method setSpeedLimit(double speedLimitPct);
    /**
     * @brief Construct a new Chassis object
     *
     * @param left The left side motors
     * @param right The right side motors
     * @param p The odometry system
     * @param trackWidth The distance from the middle of the left wheel to the middle of the right wheel
     * @param gearRatio The gear ratio (input/output)
     * @param wheelDiameter The radius of the wheels
     * @param cartridge The motor cartridge
     */
    Chassis(MotorGroup& left, MotorGroup& right, Positioner& p, double trackWidth, double gearRatio, double wheelDiameter, gearSetting cartridge);
};
#endif
