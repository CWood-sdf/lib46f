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
class PathFollowSettings
{
    typedef PathFollowSettings& chain_method;

public:
    enum class exitMode
    {
        normal,
        hold = normal,
        coast,
        nothing
    };
    bool useDistToGoal = true;
    bool turnAtStart = true;
    double virtualPursuitDist = 5.0;
    double exitDist = 1.0;
    exitMode brakeMode = exitMode::normal;
    double pathRadius = 1.0;
    double followPathDist = 16.0;
    int maxTimeIn = 10;
    chain_method setUseDistToGoal(bool v)
    {
        useDistToGoal = v;
        return *this;
    }
    chain_method setTurnAtStart(bool v)
    {
        turnAtStart = v;
        return *this;
    }
    chain_method setVirtualPursuitDist(double v)
    {
        virtualPursuitDist = v;
        return *this;
    }
    chain_method setExitDist(double v)
    {
        exitDist = v;
        return *this;
    }
    chain_method setBrakeMode(exitMode v)
    {
        brakeMode = v;
        return *this;
    }
    chain_method setPathRadius(double v)
    {
        pathRadius = v;
        return *this;
    }
    chain_method setFollowPathDist(double v)
    {
        followPathDist = v;
        return *this;
    }
    chain_method setMaxTimeIn(int v)
    {
        maxTimeIn = v;
        return *this;
    }
};
class WheelController
{
protected: // PID variables + other random things
    typedef WheelController& chain_method;

    map<double, std::function<void()>> distFns, oldFns;
    LinkedList<PidAdder> customPidsTurn;
    bool callingInDrive = false;
    std::function<PVector(PVector)> autonReversePosition;
    std::function<double(double)> autonReverseAngle;

public: // Some variables
    PID turnCtrl;
    // A public path for drawing
    VectorArr publicPath;
    bool drawArr = false;

    // Is true if afterTurn exists
    bool hasFn = false;
    // Function to be called between turning and driving
    std::function<void()> afterTurn = []() {};
    RamseteController* defaultRamsete;
    PurePursuitController* defaultPurePursuit;
    PidController* defaultPid;

public: // Constructor
    WheelController(
        Chassis* c,
        RamseteController* defRamsete,
        PurePursuitController* defPurePursuit,
        PidController* defPid,
        std::function<PVector(PVector)> reversePos,
        std::function<double(double)> reverseAngle,
        PID turnCtrl,
        double kConst = 1.0)
    {
        autonReversePosition = reversePos;
        autonReverseAngle = reverseAngle;
        defaultPurePursuit = defPurePursuit;
        defaultRamsete = defRamsete;
        defaultPid = defPid;
        path.setK(kConst);
        this->turnCtrl = turnCtrl;
        chassis = c;
        cout << chassis << endl;
    }
    std::function<void()> drawFn = []() {};
    void draw(bool)
    {
        drawFn();
    }
    Path path = Path();

public: // Some Functions
    // void addTurnPid(PidAdder a);
    // void addTurnPid(double p, double i, double d);
    // void popTopTurnPid();

    void driveTo(double x, double y);
    void backInto(double x, double y);
    virtual double botAngle();
    PVector botPos();
    // Add a function to be called at a specified distance
    void addDistFn(double dist, std::function<void()> fn);
    // Reuse the old map
    void reuseDistFns();
    void setFn(std::function<void()> fn);
    void callFn();
    void reuseFn();

    void setOldDistFns();
    void useDistFns(double dist);

private: // turnTo, with re-updating function
    virtual void turnTo(std::function<double()> angleCalc);

public: // TurnTo
    virtual void turnTo(double angle);
    typedef PathFollowSettings::exitMode exitMode;

private: // followPath vars
    PVector lastTarget;
    double exitDist = 0.0;
    exitMode BrakeMode = exitMode::normal;
    // Set to true by external threads to stop the robot
    bool exitEarly = false;
    // Is true when auto needs to be reversed
    bool reversed = false;
    // Is true when wheelbase is following path
    bool moving = false;
    // Set to true to prevent a stop exit
    bool stopExitPrev = false;
    // The radius of the path
    double pathRadius = 1.0;
    // Distance following the path
    double followPathDist = 16.0;
    // the time before exit
    int followPathMaxTimeIn = 5;

public: // exitMode
public: // followPath var editors
    bool isMoving();
    double getPathRadius();
    double getFollowPathDist();
    chain_method setFollowPathDist(double dist);
    chain_method setFollowPathMaxTimeIn(int time);
    int getFollowPathMaxTimeIn();
    // chain_method setPathRadius(double r);
    chain_method estimateStartPos(PVector v, double a);
    chain_method forceEarlyExit();
    chain_method setExitMode(exitMode m);
    chain_method setExitDist(double v);
    PVector getLastTarget();
    chain_method prevStopExit();
    chain_method setPathRadius(double r);

private: // General path follower
    Chassis* chassis;
    template <class Arr>
    size_t getNearest(Arr arr, PVector obj);
    template <class Arr>
    size_t getNearest(Arr arr, PVector obj, size_t start);

public:
    virtual void faceTarget(PVector target);
    virtual void ramseteFollow(VectorArr arr, bool isNeg);
    virtual void purePursuitFollow(VectorArr arr, bool isNeg);

    // PathFollowSettings getDefaults(){
private:
    void generalFollowTurnAtStart(VectorArr& arr, double& purePursuitDist, bool& isNeg);
    PVector generalFollowGetVirtualPursuit(PVector& pursuit, SpeedController* controller);
    double generalFollowGetDist(int& bezierIndex, SpeedController* controller, PVector& pursuit);

public:
    // }
    virtual void generalFollow(VectorArr&& arr, SpeedController* controller, bool isNeg)
    {
        generalFollow(arr, controller, isNeg);
    }
    virtual void generalFollow(VectorArr& arr, SpeedController* controller, bool isNeg);
    virtual void followPath(SpeedController* controller, VectorArr arr)
    {
        generalFollow(arr, controller, false);
    }
    virtual void backwardsFollow(SpeedController* controller, VectorArr arr)
    {
        generalFollow(arr, controller, true);
    }
    void generalDriveDistance(double dist, bool isNeg, PidController* pid);
    void driveDistance(double dist);
    void backwardsDriveDistance(double dist);
    bool isRed();
    bool isBlue();
    void setRed();
    void setBlue();
};

class MechWheelController : public WheelController
{
public: // Import variables + constructor
    // MechWheelController(motor& BL, motor& BR, posTp&, gps&) = delete;
    MechWheelController(
        Chassis* c,
        RamseteController* ramsete, PurePursuitController* purePursuit, PidController* defPid,
        std::function<PVector(PVector)> reversePos, std::function<double(double)> reverseAngle,
        PID tc, double kConst = 1.0)
      : WheelController(c, ramsete, purePursuit, defPid, reversePos, reverseAngle, tc, kConst)
    {
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
