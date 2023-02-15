#ifndef PID_H
#define PID_H
#include "vex.h"
#include <memory>
// PID.h -- use this file for a PID class
class PIDF;
// Stores numbers to add to a PID's KVals
class PidfAdder
{
    friend class PIDF;

public:
    double p, i, d, f;
    // Constructor
    // PidAdder(0.1, 0.2, 0.1);
    PidfAdder(double p, double i, double d, double f = 0.0);
    PidfAdder();
};

struct PIDF_Extension
{
    virtual ~PIDF_Extension() {}
    PIDF_Extension()
    {
    }
    virtual void manageP(double& p)
    {
    }
    virtual void manageI(double& i)
    {
    }
    virtual void manageD(double& d)
    {
    }
    virtual double getVal(double ret)
    {
        return ret;
    }
    virtual PIDF_Extension* getCopy()
    {
        return new PIDF_Extension(*this);
    }
    virtual double manageError(double in)
    {
        return in;
    }
};
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
class PIDF
{
public:
    // The values to multiply the K values by
    double p = 0.0, i = 0.0, d = 0.0, f = 0.0;
    // A simple struct that stores the multiplication values
    struct KVals
    {
        // The variables
        double p = 0.0, i = 0.0, d = 0.0, f = 0.0;
        KVals();
        // Set the values
        KVals& operator=(KVals& vals);
        // Constructor with initializer_list
        // KVals({0.1, 0.2, 0.3});
        KVals(std::initializer_list<double> vals);
        // Add a similar KVal class (Like PidAdder) to the values
        template <class T>
        KVals& operator+=(T a);
        // Subtract
        template <class T>
        KVals& operator-=(T a);
    };
    // Stores the multiplication values under k
    KVals k;

    // Variables
    double target = 0.0, error = 0.0, lastError = 0.0, iCap = 0.0, iGrowth = 0.0, iZero = 0.0;

public:
    std::shared_ptr<PIDF_Extension> manager = std::shared_ptr<PIDF_Extension>(NULL);
    /**
     * @brief Construct a new PIDF object, with zero for everything
     *
     */
    PIDF();
    /**
     * @brief Construct a new PIDF object
     *
     * @param vals The kPIDF values
     * @param mgr A reference to a manager
     * @param iCap The absolute value cap for the integral, if less than 0, allows integral to grow infinitely
     * @param iGrowthRange The absolute value error range for integral growth, if less than 0, grows infinitely
     * @param iZeroRange The range to zero the integral, if less than 0, never zeros
     */
    PIDF(KVals vals, std::shared_ptr<PIDF_Extension> mgr, double iCap = 0.0, double iGrowthRange = 0.0, double iZeroRange = 0.0);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     * @param mgr manager
     * @param iCap integral cap
     * @param iGrowthRange integral growth range
     * @param iZeroRange Integral zero range
     */
    PIDF(double p, double i, double d, std::shared_ptr<PIDF_Extension> mgr, double iCap = 0.0, double iGrowthRange = 0.0, double iZeroRange = 0.0);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     * @param f kF
     * @param mgr manager
     * @param iCap integral cap
     * @param iGrowthRange integral growth range
     * @param iZeroRange integral zero range
     */
    PIDF(double p, double i, double d, double f, std::shared_ptr<PIDF_Extension> mgr, double iCap = 0.0, double iGrowthRange = 0.0, double iZeroRange = 0.0);
    /**
     * @brief Construct a new PIDF object
     *
     * @param vals kPIDF
     * @param iCap integral cap
     * @param iGrowthRange integral growth
     * @param iZeroRange integral zero
     */
    PIDF(KVals vals, double iCap, double iGrowthRange, double iZeroRange);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     * @param iCap integral cap
     * @param iGrowthRange integral growth
     * @param iZeroRange integral zero
     */
    PIDF(double p, double i, double d, double iCap, double iGrowthRange, double iZeroRange);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     * @param f kF
     * @param iCap integral cap
     * @param iGrowthRange integral growth
     * @param iZeroRange integral zero
     */
    PIDF(double p, double i, double d, double f, double iCap, double iGrowthRange, double iZeroRange);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     * @param f kF
     */
    PIDF(double p, double i, double d, double f);
    /**
     * @brief Construct a new PIDF object
     *
     * @param p kP
     * @param i kI
     * @param d kD
     */
    PIDF(double p, double i, double d);
    /**
     * @brief Construct a new PIDF object
     *
     * @param vals kPIDF
     */
    PIDF(KVals vals);
    /**
     * @brief Copy Construct a new PIDF object
     *
     * @param v
     */
    PIDF(const PIDF& v);
    /**
     * @brief Move Construct a new PIDF object
     *
     * @param v
     */
    PIDF(PIDF&& v);
    /**
     * @brief Get the Error
     *
     * @return double
     */
    double getError();
    /**
     * @brief Prepares the PID for a new movement
     *
     */
    void resetVals();
    /**
     * @brief Sets the target value
     *
     * @param val the value to target
     */
    void setTarget(double val);
    /**
     * @brief Updates the PID with the given sensor value
     *
     * @param sensorVal sensor value
     */
    void incVals(double sensorVal);
    /**
     * @brief Get the speed value, requires the incVals has already been called
     *
     * @return double
     */
    double getVal();
    /**
     * @brief Calls incVals, then getVal
     *
     * @param sensorVal sensor value
     * @return double
     */
    double getVal(double sensorVal);
    /**
     * @brief Add kPIDF constants
     *
     * @param a the adder
     * @return PIDF& *this
     */
    PIDF& operator+=(PidfAdder a);
    /**
     * @brief Subtract kPIDF constants
     *
     * @param a the adder
     * @return PIDF& *this
     */
    PIDF& operator-=(PidfAdder a);
    /**
     * @brief Move =
     *
     * @param a
     * @return PIDF&
     */
    PIDF& operator=(PIDF&& a);
    /**
     * @brief Copy =
     *
     * @param a
     * @return PIDF&
     */
    PIDF& operator=(const PIDF& a);
    /**
     * @brief Get a copy of kPIDF
     *
     * @return KVals
     */
    KVals getKVals();
};

typedef PIDF PID;
typedef PidfAdder PidAdder;
typedef PIDF_Extension PID_Extension;

ostream& operator<<(ostream& cout, PIDF&& v);
ostream& operator<<(ostream& cout, PIDF& p);
#endif
