#ifndef INTAKE_AUTOMATION_H
#define INTAKE_AUTOMATION_H
#include "Sensors/LineCounter.h"
#include <functional>
#if BOT == 1
struct AutoIntake
{
    // The count of disks in the intake
    int count = 0;
    // The count of disks in the intake last time it was checked
    int lastCount = 0;
    // Is true when the intake is disabled
    bool disabled = false;
    // Is true when the intake is in firing mode
    bool clearingDisks = false;
    // Is true when the intake can intake more disks
    bool readyState = false;
    // Is true when the intake is in intake mode
    bool intaking = false;
    // The time of when the last firing started
    long clearStartTime = 0;
    // Is true when the intake is in the process of fixing an unstable position
    bool fixingUnstable = false;
    // The motor direction
    int direction = 0;
    // Is true when the intake is ready to tempClearStop
    bool tempClearReady = false;
    // How much tempClearStop is left
    int tempClearStop = 200;
    // Is true when the flywheel is ready to fire
    bool flywheelReady = false;
    // The count of disks that should be in the intake
    int targetCount = 0;
    // The sensors
    vector<std::function<bool()>> sensors;
    // The mask of the sensors
    int diskMask = 0;
    // The mask of the sensors last time it was checked
    int lastMask = 0;
    AutoIntake(vector<std::function<bool()>> sensors);
    /**
     * @brief For internal use only, stores all the sensor values in a mask for easy comparison
     *
     */
    void makeMask();
    /**
     * @brief Checks if the disks are ready for intaking
     *
     * @return true yes they are
     * @return false no they are not
     */
    bool stable();
    /**
     * @brief Checks if the disks can be moved to a "stable" position
     *
     * @return true yes they can
     * @return false no they cant
     */
    bool fixableUnstable();
    /**
     * @brief Disables the auto intake
     *
     */
    void disable();
    /**
     * @brief Enables the auto intake
     *
     */
    void enable();
    /**
     * @brief Updates the values of the auto intake
     *
     * @param flywheelReady should be true if the flywheel is ready to fire
     */
    void updateValues(bool flywheelReady);
    /**
     * @brief Checks if the intake has disks
     *
     * @return true
     * @return false
     */
    bool hasDisks();
    /**
     * @brief Set the intake to fire mode
     *
     */
    void setFiring();
    /**
     * @brief Set the intake to intake mode
     *
     */
    void intake();
    /**
     * @brief Set intaking for multiple disks
     *
     */
    void intakeMultiple(int count);
    /**
     * @brief Return true if the intake should move up
     *
     * @return true
     * @return false
     */
    bool spinMotor();
    /**
     * @brief Return true if the intake should move down
     *
     * @return true
     * @return false
     */
    bool reverseMotor();
    /**
     * @brief Waits for intake to be done firing
     *
     */
    void waitForFiring();
    /**
     * @brief Waits for intake to be done intaking
     *
     */
    void waitForIntake();
    /**
     * @brief Sets the intake to be ready for auton
     *
     */
    void autonInit();
    /**
     * @brief Draws the intake state
     *
     */
    void drawState(bool);
};
#elif BOT == 2
struct AutoIntake
{
    LineCounter& counter;
    bool enabled = true;
    MotorGroup& slingMot;
    MotorGroup& intakeMot;
    pneumatics& release;
    std::function<bool()> atBack;
    bool isAtBack = false;
    bool pneumaticsReleaseState = false;
    int timeSinceRelease = 0;
    int targetCount = 0;
    bool reversed = false;
    int continueTime = 0;
    int continueDirection = 0;
    int lastCount = 0;
    bool forward = false;
    AutoIntake(LineCounter& counter, MotorGroup& sling, MotorGroup& intake, pneumatics& release, std::function<bool()> ready, bool pneumaticsReleaseState = false);
    void disable();
    void enable();
    void updateValues();
    void setFiring();
    void intake();
    void intakeMultiple(int count);
    void reverseMotor();
    void stopDriverSpins();
    void setCount(int count);
    void moveForward();
    void decreaseCount();
};
#endif
#endif
