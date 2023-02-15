// #include "EPA_Tracker.h"
#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H
#include "Sensors/LineCounter.h"
#include "Sensors/PotDial.h"
// #include "Sensors/Distance.h"
#include "Flywheel/Controller.h"
#include "IntakeAutomation.h"
#include "Odometry/Controllers.h"
#include "Sensors/VisionOdom.h"
#include "Sensors/Wrappers/Encoder.h"
/*************************************

Motors

*************************************/
// //Front Left Wheel (FL)
// extern motor FL;
// //Front Right Wheel (FR)
// extern motor FR;
// //Back Left Wheel (BL)
// extern motor BL;
// //Back Right Wheel (BR)
// extern motor BR;
// //Middle Left Wheel (ML)
// extern motor ML;
// //Middle Right Wheel (MR)
// extern motor MR;

// extern Pto leftPto;
// extern Pto rightPto;

// extern motor_group Left;
// extern motor_group Right;

// extern motor flyWheelMot;
// extern motor flywheel2;

extern MotorGroup intake;
#if BOT == 1
extern MotorGroup flywheelNm;
extern FlywheelTBHEncoder flyTBH;
#elif BOT == 2
extern MotorGroup sling;
#endif

extern MotorGroup leftWheels;
extern MotorGroup rightWheels;

extern Chassis chassis;
extern PurePursuitController purePursuit;
extern RamseteController ramsete;
extern BasicPidController pidController;

/*************************************

Pneumatics

*************************************/
extern pneumatics endgame;
extern pneumatics goalRaise;
// extern pneumatics pto;

/*************************************

Sensors

*************************************/

// Three wire expander
extern triport Expander;

// gps
// extern gps GPS;

extern optical rachetColor;

extern AutoIntake intakeController;
#if BOT == 1
extern LineCounter intakeBottom;
extern vex::distance intakeMiddle;
extern LineCounter intakeTop;
#elif BOT == 2
extern LineCounter intakeCounter;
#endif
// Distance goalFront;
// Distance goalBack;

/*************************************

Odometry

*************************************/

// VisionOdom visionTest;

// Positioner init
typedef Positioner posTp;

// extern GPS_Share share;

extern Positioner positioner;
// Wheel controller
extern WheelController wc;

/*************************************

Autonomous System Controllers

*************************************/
#if BOT == 1
void graphFlywheelPID(bool remake);
void graphFlywheelTBH(bool remake);
#endif

void testDriveConfiguration();

void testDeviceConnection();
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
#endif
