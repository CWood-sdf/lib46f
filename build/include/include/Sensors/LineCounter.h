
#ifndef LINECOUNTER_H
#define LINECOUNTER_H

#include "Sensors/PotDial.h"
// Oh, this lovely class
// It just always works (unless there's a sun there)

// Class that uses a line tracker to count objects
class LineCounter {

    static const thread updater;
    friend class LineGroup;
    line* sensor;
    bool isActive = false;
    static const int highThresholdPolycarb = 75;
    static const int lowThresholdPolycarb = 55;
    static const int highThresholdAir = 15;
    static const int lowThresholdAir = 13;
    int startThreshold = highThresholdAir;
    int lowThreshold = lowThresholdAir;
    int threshold = startThreshold;
    bool wasActiveLast = false;
    unsigned int countOut = 0;
    unsigned int countIn = 0;
    bool fHit = false;

    static inline vector<LineCounter*> instances = {};
    /**
     * @brief Updates the line counter
     *
     */
    void update();

public:
    LineCounter(const LineCounter&) = delete;
    /**
     * @brief Construct a new Line Counter object
     *
     * @param se The sensor to use
     * @param throughPolycarb Whether the sensor is looking through polycarbonate
     */
    LineCounter(line& se, bool throughPolycarb = false);
    /**
     * @brief Construct a new Line Counter object
     *
     * @param p The port to use
     * @param throughPolycarb Whether the sensor is looking through polycarbonate
     */
    LineCounter(triport::port& p, bool throughPolycarb = false);
    /**
     * @brief Returns true for the first time this function is called after an object enters the field of view
     *
     * @return true
     * @return false
     */
    bool firstHit();
    /**
     * @brief Returns true if a object is in the field of view
     *
     * @return true
     * @return false
     */
    bool active();
    /**
     * @brief Resets the line counter count
     */
    void reset();
    /**
     * @brief Get the number of objects that have entered then left the field of view
     *
     * @return int
     */
    int getCountOut();
    /**
     * @brief Get the number of objects that have entered the field of view
     *
     * @return int
     */
    int getCountIn();
    /**
     * @brief Returns true if a object is in the field of view
     *
     * @return true
     * @return false
     */
    bool pressing();
    /**
     * @brief Returns the raw sensor value
     *
     * @return int
     */
    int rawData();
    /**
     * @brief Set the count of objects
     *
     * @param count The new count
     */
    void setCount(int count);
    /**
     * @brief Draw a list of all the line counters on the brain screen
     *
     */
    static void listVals(bool);
};

#endif // LINECOUNTER_H
