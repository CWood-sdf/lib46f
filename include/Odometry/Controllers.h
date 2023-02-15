#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include "EPA_Wheel_Control.h"
#ifndef CHAIN
#define CHAIN return *this;
#endif
// That should be a sufficient path generator

double sign(double v);
class SpeedController
{
public:
    PathFollowSettings settings;

    struct Input
    {
        PVector target;
        PVector position;
        double angleTarget;
        double currentAngle;
        double dist;
        Path::El targetPt;
        Chassis* chassis;
        // bool isNeg;
    };
    enum class AngularVel
    {
        curvature,
        pctDiff,
        radps
    };
    enum class ForwardVel
    {
        pct,
        inps
    };

    virtual void init();

    typedef pair<pair<double, ForwardVel>, pair<double, AngularVel>> followToRet;
    virtual followToRet followTo(Input& input);
    followToRet followTo(Input&& v)
    {
        return followTo(v);
    }

    virtual void deInit();
    SpeedController()
    {
    }
};
class PurePursuitController : public SpeedController
{
    // Get ctrl kP up to 6.25
    PID ctrl = PID();

public:
    followToRet followTo(Input& input) override;
    void init() override;
    PurePursuitController(PID input);
    PurePursuitController(PID input, PathFollowSettings settings);
    PurePursuitController() = delete;
};
class RamseteController : public SpeedController
{
    // beta ~ p-term
    //[b > 0]
    // zeta ~ d-term
    //[0 < zeta < 1]
    double beta; // rad^2/in^2
    double zeta; // 1/rad
public:
    followToRet followTo(Input& input) override;
    //[b > 0], [0 < zeta < 1]
    RamseteController(double beta, double zeta);
    RamseteController(double beta, double zeta, PathFollowSettings settings);
    RamseteController() = delete;
};
class PidController : public SpeedController
{
    PID ctrl;
    PID slave;

public:
    void init() override;
    PidController(PID, PID);
    PidController(PID, PID, PathFollowSettings settings);
    followToRet followTo(Input& input) override;
    PidController() = delete;
};
#endif
