#include "Filters.h"
#include "Odometry/PID.h"
#include "Sensors/Wrappers/Encoder.h"
#include <algorithm>
#include <cstdint>
#if BOT != 2
// Determines if a flywheel is at the proper velocity
// Also calculates time between steps so that I don't have to
//   Manage it elsewhere in the program
class Settled
{
    SMA derivFilter = SMA(5, 0);
    double maxDeriv;
    timer time;
    double maxErr;
    double prevErr = 0.0;
    double maxSleep = 500;
    uint32_t lastTimeStep;
    LinkedList<double> lastDerivs;
    bool isSettled;

public:
    Settled(double me, double md, double ms = 500)
    {
        maxErr = me;
        maxDeriv = md;
        maxSleep = ms;
    }
    bool settled(double err)
    {
        lastTimeStep = time;
        int32_t timeStep = time;
        time.reset();
        double deriv = (err - prevErr) / (double)timeStep;
        prevErr = err;
        isSettled = false;
        if (timeStep > 1000)
        {
            return false;
        }

        derivFilter.update(deriv);
        lastDerivs.pushBack(derivFilter);
        if (lastDerivs.size() > 5)
        {
            lastDerivs.popBase();
        }
        // cout << abs(derivFilter) << "\n";
        isSettled = abs(derivFilter) < maxDeriv && abs(err) < maxErr;
        return isSettled;
    }
    bool stableSpeed()
    {
        // If all the lastDerivs are less than maxDeriv, return true
        for (auto deriv : lastDerivs)
        {
            if (abs(deriv) > maxDeriv)
            {
                return false;
            }
        }
        return true;
    }
    bool settled()
    {
        return isSettled;
    }
    uint32_t timeStep()
    {
        return lastTimeStep;
    }
    double getDeriv()
    {
        return derivFilter;
    }
};
struct FlywheelDebugEl
{
    double error;
    double measuredVel;
    double filterVel;
    double targetVel;
    double sentVel;
    double deriv;
    static const int size;
    void set(double err, double raw, double filtRetVal, double targ, double sent, double deriv)
    {
        error = err;
        measuredVel = raw;
        filterVel = filtRetVal;
        targetVel = targ;
        sentVel = sent;
        this->deriv = deriv;
    }
    // Sussy version
    void set(double* arr)
    {
        for (int i = 0; i < size; i++)
        {
            ((double*)this)[i] = arr[i];
        }
    }
};

class FlywheelTBHEncoder
{
    Encoder en;
    MotorGroup& mots;
    EMA filter;
    MinMaxFilter minMaxFilter;
    WeightFilter weightFilter;
    SMA sma;
    vector<double> velTargets = {550};
    vector<double> initialTbh = {10};
    double tbh = 0;
    double gain;
    Settled velCheck = Settled(10, 100, 500);
    int target;
    FlywheelDebugEl debug;
    bool hasTarget = false;
    bool disabled = false;

public:
    double maxRateDrop = 2;
    double maxRateGain = 4;
    FlywheelTBHEncoder(MotorGroup& m, Encoder en);
    FlywheelTBHEncoder(MotorGroup& m);
    void setTarget(int i);
    void addTarget(double t);
    void setTargetSpeed(double t);
    void step();
    void graph(bool);
    void init();
    bool ready();
    void setDisabled(bool d);
};
class EMA_D : public PIDF_Extension
{
    EMA dFilter = EMA(0.7, 0);

public:
    void manageD(double& d) override
    {
        dFilter.update(d);
        // d = dFilter.value();
    }
};
// class FlywheelPID : public Empty {
//   encoder* en;
//   motor& mots;
//   EMA filter = EMA(0.1, 0);
//   vector<double> velTargets = {100};
//   Settled velCheck = Settled(100, 100 * 100, 500);
//   EMA_D manager;
//   PIDF ctrl;
//   FlywheelDebugEl debug;
//   int target = 0;
// public:
//   FlywheelPID(motor& m, vex::encoder& e);
//   void setTarget(int i);
//   void step() override;
//   void graph(bool);
//   void initPID();
// };
#endif
