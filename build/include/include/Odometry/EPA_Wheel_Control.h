// EPA_WheelControl.h -- Use this file to control the wheel base
//      to go to specified positions
// Todo: clean up stuff
#ifndef EPA_WHEEL_CONTROL_H
#define EPA_WHEEL_CONTROL_H
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "Chassis.h"
#include <deque>
#ifdef DEBUG
#warning "DEBUG already defined"
#else
#define DEBUG
#endif
// The basic wheel controller
class RamseteController;
class PidController;
class PurePursuitController;
class PathFollowSettings {
    typedef PathFollowSettings& chain_method;

public:
    /**
     * @brief The exitMode enum
     *
     */
    enum class exitMode {
        // Apply hold brake
        normal,
        hold = normal,
        // Apply coast brake
        coast,
        // No brake
        nothing
    };
    /**
     * @brief Whether to exit on distance to goal point, or forward distance
     *
     */
    bool useDistToGoal = true;
    /**
     * @brief Whether to turn at the start of the path
     *
     */
    bool turnAtStart = true;
    /**
     * @brief The distance to add on to the end of the path
     *
     */
    double virtualPursuitDist = 5.0;
    /**
     * @brief The distance to exit the path from
     *
     */
    double exitDist = 1.0;
    /**
     * @brief the exitMode
     *
     */
    exitMode brakeMode = exitMode::normal;
    /**
     * @brief The distance to follow the path from
     *
     */
    double followPathDist = 16.0;
    /**
     * @brief The time in ms to allow the robot to be in the exitDist before exiting
     *
     */
    int timeIn = 10;
    /**
     * @brief Set useDistToGoal
     *
     * @param v the new value
     * @return chain_method
     */
    chain_method setUseDistToGoal(bool v) {
        useDistToGoal = v;
        return *this;
    }
    /**
     * @brief Set turnAtStart
     *
     * @param v the new value
     * @return chain_method
     */
    chain_method setTurnAtStart(bool v) {
        turnAtStart = v;
        return *this;
    }
    /**
     * @brief Set virtualPursuitDist
     *
     * @param distIn the distance in inches
     * @return chain_method
     */
    chain_method setVirtualPursuitDist(double distIn) {
        virtualPursuitDist = distIn;
        return *this;
    }
    /**
     * @brief Set exitDist
     *
     * @param distIn the distance in inches
     * @return chain_method
     */
    chain_method setExitDist(double distIn) {
        exitDist = distIn;
        return *this;
    }
    /**
     * @brief Set brakeMode
     *
     * @param v the new value
     * @return chain_method
     */
    chain_method setBrakeMode(exitMode v) {
        brakeMode = v;
        return *this;
    }
    /**
     * @brief Set followPathDist
     *
     * @param distIn the new value
     * @return chain_method
     */
    chain_method setFollowPathDist(double distIn) {
        followPathDist = distIn;
        return *this;
    }
    /**
     * @brief Set timeIn
     *
     * @param timeMs The time in ms
     * @return chain_method
     */
    chain_method setTimeIn(int timeMs) {
        timeIn = timeMs;
        return *this;
    }
};
class WheelController {
private: // PID variables + other random things
    map<double, std::function<void()>> distFns, oldFns;
    bool callingInDrive = false;
    std::function<PVector(PVector)> autonReversePosition;
    std::function<double(double)> autonReverseAngle;

    // Function to be called between turning and driving
    std::function<void()> afterTurn = []() {};

public: // Some variables
    /**
     * @brief The turn control pid
     *
     */
    PID turnCtrl;
    /**
     * @brief Holds a copy of the current path being followed
     *
     */
    VectorArr publicPath;
    /**
     * @brief Whether publicPath holds a valid path
     *
     */
    bool drawArr = false;
    /**
     * @brief The default ramsete controller
     *
     */
    RamseteController* defaultRamsete;
    /**
     * @brief The default pure pursuit controller
     *
     */
    PurePursuitController* defaultPurePursuit;
    /**
     * @brief The default pid controller
     *
     */
    PidController* defaultPid;
    /**
     * @brief Construct a new Wheel Controller object
     *
     * @param c the chassis
     * @param defRamsete the default ramsete controller
     * @param defPurePursuit the default pure pursuit controller
     * @param defPid the default pid controller
     * @param reversePos the function to reverse the position for autons
     * @param reverseAngle the function to reverse the angle for autons
     * @param turnCtrl the turn control pid
     * @param kConst the k constant for the path
     */
    WheelController(
        Chassis* c,
        RamseteController* defRamsete,
        PurePursuitController* defPurePursuit,
        PidController* defPid,
        std::function<PVector(PVector)> reversePos,
        std::function<double(double)> reverseAngle,
        PID turnCtrl,
        double kConst = 1.0) {

        autonReversePosition = reversePos;
        autonReverseAngle = reverseAngle;
        defaultPurePursuit = defPurePursuit;
        defaultRamsete = defRamsete;
        defaultPid = defPid;
        path.setK(kConst);
        this->turnCtrl = turnCtrl;
        chassis = c;
    }
    /**
     * @brief The Path for making paths
     *
     */
    Path path = Path();

    /**
     * @brief Drive the robot to (x,y)
     *
     * @param x the x coordinate
     * @param y the y coordinate
     */
    void driveTo(double x, double y);
    /**
     * @brief Drive the robot backwars to (x,y)
     *
     * @param x the x coordinate
     * @param y the y coordinate
     */
    void backInto(double x, double y);
    /**
     * @brief Returns the robots heading
     *
     * @return double
     */
    double botAngle();
    /**
     * @brief Returns the robots position
     *
     * @return PVector
     */
    PVector botPos();
    /**
     * @brief Add a function to be called within a specified distance
     *
     * @param dist the distance in inches
     * @param fn the function to be called
     */
    void addDistFn(double dist, std::function<void()> fn);
    /**
     * @brief Reuse the distance functions from the last path
     *
     */
    void reuseDistFns();
    /**
     * @brief Set the function to be called after turning to face the path
     *
     * @param fn
     */
    void setAfterTurnFn(std::function<void()> fn);

private: // turnTo, with re-updating function
    void setOldDistFns();
    void useDistFns(double dist);
    virtual void turnTo(std::function<double()> angleCalc);

public: // TurnTo
    /**
     * @brief Turn the robot to a specific angle
     *
     * @param angle
     */
    virtual void turnTo(double angle);

private: // followPath vars
    // Set to true by external threads to stop the robot
    bool exitEarly = false;
    // Is true when auto needs to be reversed
    bool reversed = false;
    // Is true when wheelbase is following path
    bool moving = false;
    // Set to true to prevent a stop exit
    bool stopExitPrev = false;

public:
    /**
     * @brief Returns true if the robot is moving
     *
     * @return true
     * @return false
     */
    bool isMoving();
    /**
     * @brief Set the starting position for an auton
     *
     * @param v The position
     * @param a The heading
     */
    void estimateStartPos(PVector v, double a);
    /**
     * @brief Force the path folowing to end instantly
     *
     */
    void forceEarlyExit();
    /**
     * @brief Prevent the robot from exiting when stopped
     *
     */
    void prevStopExit();

private: // General path follower
    Chassis* chassis;
    template <class Arr>
    size_t getNearest(Arr arr, PVector obj);
    template <class Arr>
    size_t getNearest(Arr arr, PVector obj, size_t start);

public:
    /**
     * @brief Face a target
     *
     * @param target the target
     */
    virtual void faceTarget(PVector target);
    /**
     * @brief Follow a path with the ramsete controller
     *
     * @param arr The control point array
     * @param isNeg Whether to follow backwards
     */
    virtual void ramseteFollow(VectorArr arr, bool isNeg);
    /**
     * @brief Follow a path with the pure pursuit controller
     *
     * @param arr The control point array
     * @param isNeg Whether to follow backwards
     */
    virtual void purePursuitFollow(VectorArr arr, bool isNeg);

private:
    void generalFollowTurnAtStart(VectorArr& arr, double& purePursuitDist, bool& isNeg);
    PVector generalFollowGetVirtualPursuit(PVector& pursuit, SpeedController* controller);
    double generalFollowGetDist(int& bezierIndex, SpeedController* controller, PVector& pursuit);
    virtual void generalFollow(VectorArr&& arr, SpeedController* controller, bool isNeg) {
        generalFollow(arr, controller, isNeg);
    }
    virtual void generalFollow(VectorArr& arr, SpeedController* controller, bool isNeg);
    void generalDriveDistance(double dist, bool isNeg, PidController* pid);

public:
    /**
     * @brief Follow a path with the specified controller
     *
     * @param controller the controller to use
     * @param arr the control point array
     */
    virtual void followPath(SpeedController* controller, VectorArr arr) {
        generalFollow(arr, controller, false);
    }
    /**
     * @brief Follow a path backwards with the specified controller
     *
     * @param controller the controller to use
     * @param arr the control point array
     */
    virtual void backwardsFollow(SpeedController* controller, VectorArr arr) {
        generalFollow(arr, controller, true);
    }
    /**
     * @brief Drive a certain distance
     *
     * @param dist the distance in inches
     */
    void driveDistance(double dist);
    /**
     * @brief Drive a certain distance backwards
     *
     * @param dist the distance in inches
     */
    void backwardsDriveDistance(double dist);
    /**
     * @brief Drive a certain distance with a specified controller
     *
     * @param dist the distance in inches
     * @param pid the controller to use
     */
    void driveDistance(double dist, PidController* pid) {
        generalDriveDistance(dist, false, pid);
    }
    /**
     * @brief Drive a certain distance backwards with a specified controller
     *
     * @param dist the distance in inches
     * @param pid the controller to use
     */
    void backwardsDriveDistance(double dist, PidController* pid) {
        generalDriveDistance(dist, true, pid);
    }
    /**
     * @brief Returns true if the robot has been set to the red alliance
     *
     * @return true
     * @return false
     */
    bool isRed();
    /**
     * @brief Returns true if the robot has been set to the blue alliance
     *
     * @return true
     * @return false
     */
    bool isBlue();
    /**
     * @brief Sets the robot alliance to red
     *
     */
    void setRed();
    /**
     * @brief Sets the robot alliance to blue
     *
     */
    void setBlue();
};

class MechWheelController : public WheelController {
public: // Import variables + constructor
    // MechWheelController(motor& BL, motor& BR, posTp&, gps&) = delete;
    MechWheelController(
        Chassis* c,
        RamseteController* ramsete, PurePursuitController* purePursuit, PidController* defPid,
        std::function<PVector(PVector)> reversePos, std::function<double(double)> reverseAngle,
        PID tc, double kConst = 1.0)
      : WheelController(c, ramsete, purePursuit, defPid, reversePos, reverseAngle, tc, kConst) {
    }

public:
    void moveAt(double angle, double speed, double turnSpeed);
    void followPath(VectorArr arr, double targetAngle);
    void driveTo(PVector pos, double finalAngle);
    void driveTo(double x, double y, double finalAngle);
    void backInto(double x, double y, double finalAngle);
    void backInto(PVector pos, double finalAngle);
    virtual void backwardsFollow(VectorArr arr, double targetAngle);
};

#endif
