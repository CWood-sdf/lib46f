#ifndef POTDIAL_H
#define POTDIAL_H

#include "Odometry/EPA_Tracker.h"
class PotDial;
class SelectorArr {
    typedef function<void()> FnTp;
    static inline FnTp emptyFn = []() { cout << "SelectorArr: Empty function called" << endl; };
    static inline vector<PotDial*> boundDials = {};
    static inline map<vector<int>, FnTp> toVal = {};
    vector<int> count;
    FnTp fn = emptyFn;
    friend class PotDial;
    static void addDial(PotDial* dial);

public:
    /**
     * @brief Construct a new Selector Arr object
     *
     * @param count The desired value of the dials
     * @param fn The function to be called when the dials are set to the desired value
     */
    SelectorArr(vector<int> count, FnTp fn);
    /**
     * @brief Construct a new Selector Arr object
     *
     * @param count The desired value of the dials
     */
    SelectorArr(vector<int> count);
    /**
     * @brief Construct a new Selector Arr object
     *
     */
    SelectorArr() {}
    /**
     * @brief Set the function to be called when the dials are set to the desired value
     *
     * @param fn
     */
    void attachFn(FnTp fn);
    /**
     * @brief Set the desired value of the dials
     *
     * @param count The desired value of the dials
     */
    void setArray(vector<int> count);
    /**
     * @brief Get the function to be called given the dials' current values
     *
     * @return FnTp
     */
    static FnTp getVal();
};
class PotDial {
    const int ticks;
    pot* sensor;
    double baseVal = 0.0;
    int range = 250;

    PotDial(int tickAmnt, bool addToSelector) : ticks(tickAmnt) {
        if (addToSelector) {
            SelectorArr::addDial(this);
        }
    }
    PotDial(int tickAmnt, int rng, bool addToSelector) : PotDial(tickAmnt, addToSelector) {
        range = rng;
    }
    PotDial(int tickAmnt, int rng, double baseVal, bool addToSelector = true) : PotDial(tickAmnt, rng, addToSelector) {
        this->baseVal = baseVal;
    }

public:
    /**
     * @brief Construct a new Pot Dial object
     *
     * @tparam Args
     * @param sensor The potentiometer to be used
     * @param otherArgs The private constructor arguments
     */
    template <typename... Args>
    PotDial(pot& sensor, Args... otherArgs) : PotDial(otherArgs...) {
        this->sensor = &sensor;
    }
    /**
     * @brief Construct a new Pot Dial object
     *
     * @tparam Args
     * @param p The port to be used
     * @param args The private constructor arguments
     */
    template <typename... Args>
    PotDial(triport::port& p, Args... args) : PotDial(args...) {
        sensor = new pot(p);
    }
    /**
     * @brief Return the value from the potentiometer
     *
     * @return int
     */
    int getAmnt();

    int raw() {
        return sensor->angle(degrees);
    }
};

#endif // POTDIAL_H
