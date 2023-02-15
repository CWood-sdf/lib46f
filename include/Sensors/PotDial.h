#ifndef POTDIAL_H
#define POTDIAL_H

#include "Odometry/EPA_Tracker.h"
typedef triport::port port;
class PotDial;
class SelectorArr
{
    typedef function<void()> FnTp;
    static inline FnTp emptyFn = []()
    { cout << "SelectorArr: Empty function called" << endl; };
    static inline vector<PotDial*> boundDials = {};
    static inline map<vector<int>, FnTp> toVal = {};
    vector<int> count;
    friend class PotDial;
    static void addDial(PotDial* dial);

public:
    SelectorArr(vector<int> count, FnTp fn);
    SelectorArr(vector<int> count);
    SelectorArr() {}
    void attachFn(FnTp fn);
    static FnTp getVal();
};
class PotDial
{
    const int ticks;
    pot* sensor;
    double baseVal = 0.0;
    int range = 250;

    PotDial(int tickAmnt, bool addToSelector) : ticks(tickAmnt)
    {
        if (addToSelector)
        {
            SelectorArr::addDial(this);
        }
    }
    PotDial(int tickAmnt, int rng, bool addToSelector) : PotDial(tickAmnt, addToSelector)
    {
        range = rng;
    }
    PotDial(int tickAmnt, int rng, double baseVal, bool addToSelector = true) : PotDial(tickAmnt, rng, addToSelector)
    {
        this->baseVal = baseVal;
    }

public:
public:
    template <typename... Args>
    PotDial(pot& sensor, Args... otherArgs) : PotDial(otherArgs...)
    {
        this->sensor = &sensor;
    }
    template <typename... Args>
    PotDial(port& p, Args... args) : PotDial(args...)
    {
        sensor = new pot(p);
    }
    int getAmnt();
};

#endif // POTDIAL_H
