#include "PID.h"
PidfAdder::PidfAdder(double p, double i, double d, double f)
{
    this->p = p;
    this->i = i;
    this->d = d;
    this->f = f;
}
PidfAdder::PidfAdder()
{
}

// Some nice simple math
/********************************
.                     ____   ___    ______
ENSURE THAT THERE ARE |   \  | |   |  __  |
.                     | |\ \ | |   | |  | |
.                     | | \ \| |   | |__| |
.                     |_|  \___|   |______|
NON-DOUBLE TYPE VARIABLES BEFORE THE DOUBLE TYPE VARIABLES
    (This is for hacky pointer copy constructor)
********************************/

PIDF::KVals::KVals()
{
    p = i = d = f = 0.0;
}
// Set the values
PIDF::KVals& PIDF::KVals::operator=(KVals& vals)
{
    p = vals.p;
    i = vals.i;
    d = vals.d;
    f = vals.f;
    return *this;
}
// Constructor with initializer_list
// KVals({0.1, 0.2, 0.3});
PIDF::KVals::KVals(std::initializer_list<double> vals)
{
    double* ths = (double*)this;
    for (double val : vals)
    {
        *ths = val;
        ++ths;
    }
}
// Add a similar KVal class (Like PidAdder) to the values
template <class T>
PIDF::KVals& PIDF::KVals::operator+=(T a)
{
    p += a.p;
    i += a.i;
    d += a.d;
    f += a.f;
    return *this;
}
// Subtract
template <class T>
PIDF::KVals& PIDF::KVals::operator-=(T a)
{
    p -= a.p;
    i -= a.i;
    d -= a.d;
    f -= a.f;
    return *this;
}
PIDF::PIDF()
{
}
PIDF::PIDF(KVals vals, std::shared_ptr<PIDF_Extension> mgr, double iCap, double iGrowthRange, double iZeroRange) : PIDF(vals, iCap, iGrowthRange, iZeroRange)
{
    manager = mgr;
}
PIDF::PIDF(double p, double i, double d, std::shared_ptr<PIDF_Extension> mgr, double iCap, double iGrowthRange, double iZeroRange) : PIDF(p, i, d, 0.0, iCap, iGrowthRange, iZeroRange)
{
    manager = mgr;
}
PIDF::PIDF(double p, double i, double d, double f, std::shared_ptr<PIDF_Extension> mgr, double iCap, double iGrowthRange, double iZeroRange) : PIDF(p, i, d, f, iCap, iGrowthRange, iZeroRange)
{
    manager = mgr;
}
// Constructors
PIDF::PIDF(KVals vals, double iCap, double iGrowthRange, double iZeroRange) : k(vals)
{
    this->iCap = iCap;
    iGrowth = iGrowthRange;
    iZero = iZeroRange;
}
PIDF::PIDF(double p, double i, double d, double iCap, double iGrowthRange, double iZeroRange) : PIDF(KVals({p, i, d, 0.0}))
{
    this->iCap = iCap;
    iGrowth = iGrowthRange;
    iZero = iZeroRange;
}
PIDF::PIDF(double p, double i, double d, double f, double iCap, double iGrowthRange, double iZeroRange) : PIDF(KVals({p, i, d, f}))
{
    this->iCap = iCap;
    iGrowth = iGrowthRange;
    iZero = iZeroRange;
}
PIDF::PIDF(double p, double i, double d, double f) : PIDF(KVals({p, i, d, f})) {}
PIDF::PIDF(double p, double i, double d) : PIDF(KVals({p, i, d, 0.0})) {}
PIDF::PIDF(KVals vals) : k(vals)
{
}
// HACK
PIDF::PIDF(const PIDF& v)
{
    // i hate this but it should work
    double* doubles = (double*)&v;
    double* thisPtr = (double*)this;
    // cout << doubles << ", " << thisPtr << endl;
    int size = (sizeof(PIDF) - sizeof(std::shared_ptr<PIDF_Extension>)) / sizeof(double);
    // cout << size << endl;
    for (int i = 0; i < size; i++)
    {
        thisPtr[i] = doubles[i];
    }

    // Clear any error build up
    resetVals();
    // cout << (bool)v.manager << endl;
    if (v.manager)
    {
        manager.reset(v.manager->getCopy());
    }
}
PIDF::PIDF(PIDF&& v)
{
    operator=(std::move(v));
}
// Get the error
double PIDF::getError()
{
    return error;
}
// Clear out the previous PID usage
void PIDF::resetVals()
{
    error = 0.0;
    p = i = d = 0.0;
}
// Set the target value of the PID
void PIDF::setTarget(double val)
{
    resetVals();
    target = val;
}
// Apply the error
void PIDF::incVals(double sensorVal)
{

    if (manager)
    {
        error = manager->manageError(target - sensorVal);
    }
    else
    {
        error = target - sensorVal;
    }
    p = error;
    if (abs(error) <= iGrowth || iGrowth < 0.0)
    {
        i += error;
    }
    if (abs(i) > abs(iCap) && abs(iCap) >= 0.0)
    {
        i = iCap;
    }
    if (abs(error) <= iZero && iZero >= 0.0)
    {
        i = 0.0;
    }
    d = error - lastError;
    if (manager)
    {
        manager->manageP(p);
        manager->manageI(i);
        manager->manageD(d);
    }
    lastError = error;
}
// Get the speed value given that error has already been applied
double PIDF::getVal()
{
    double pInc = k.p * p;
    double iInc = k.i * this->i;
    double dInc = k.d * this->d;
    if (manager)
    {
        return manager->getVal(pInc + iInc + dInc);
    }
    else
    {
        return pInc + iInc + dInc;
    }
}
// Apply error, then return getVal()
double PIDF::getVal(double sensorVal)
{
    this->incVals(sensorVal);
    return getVal();
}
// Add a PidAdder
PIDF& PIDF::operator+=(PidfAdder a)
{
    k += a;
    return *this;
}
// Subtract a PidAdder
PIDF& PIDF::operator-=(PidfAdder a)
{
    k -= a;
    return *this;
}
PIDF& PIDF::operator=(PIDF&& a)
{
    return operator=(a);
    CHAIN
}
// Directly taken from the copy constructor
PIDF& PIDF::operator=(const PIDF& a)
{
    // i hate this but it should work
    double* doubles = (double*)&a;
    double* thisPtr = (double*)this;
    // cout << doubles << ", " << thisPtr << endl;
    int size = (sizeof(PIDF) - sizeof(std::shared_ptr<PIDF_Extension>)) / sizeof(double);
    // cout << size << endl;
    for (int i = 0; i < size; i++)
    {
        thisPtr[i] = doubles[i];
    }

    // Clear any error build up
    resetVals();
    // cout << (bool)a.manager << endl;
    if (a.manager)
    {
        manager.reset(a.manager->getCopy());
    }
    CHAIN
}
PIDF::KVals PIDF::getKVals()
{
    return k;
}

ostream& operator<<(ostream& cout, PIDF&& v)
{
    cout << v.getKVals().p << ", " << v.getKVals().i << ", " << v.getKVals().d << ", " << v.getKVals().f << endl;
    return cout;
}
ostream& operator<<(ostream& cout, PIDF& p)
{
    cout << p.k.p << ", " << p.k.i << ", " << p.k.d << ", " << p.k.f << endl;
    return cout;
}
