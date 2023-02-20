#ifndef macros
#define macros 1
#pragma once
// #ifndef waitUntil
#define waitUntil(condition) \
    do {                     \
        wait(5, msec);       \
    } while (!condition)
// #endif //waitUntil
//
////#ifndef sl
// #d efine sl                                                           \
//  vex::task::sleep (300);
////#endif //sl

// #define comma << ", " <<
//
////#ifndef cubout
// #d efine cubeout                                                      \
//  vex::task::sleep (500);
////#endif //co

// #ifndef startRepeat
#define startRepeat(start, end) \
    for (int i = start; i < end; i++)
// #endif

// #ifndef repeat
#define repeat(iterations) \
    for (int iterator = 0; iterator < iterations; iterator++)
// #endif //repeat

//#define to_infinity_and_beyond                                       \
//  while(1)
//

// #define EMPTY_ZONE                                                 \
//   {n},{n},{n},{n},{n},{n},{n},{n}

// #define fullRow(ofWhat)                                             \
//   {ofWhat},{ofWhat},{ofWhat},{ofWhat},{ofWhat},{ofWhat},{ofWhat},{ofWhat}
// #define EMPTY_ROW                                                    \
//   {n},{n},{n},{n},{n},{n},{n},{n},/**/{n},{n},{n},{n},{n},{n},{n},{n},/**/{n},{n},{n},{n},{n},{n},{n},{n},   \
//     /**/{n},{n},{n},{n},{n},{n},{n},{n},/**/{n},{n},{n},{n},{n},{n},{n},{n},/**/{n},{n},{n},{n},{n},{n},{n},{n}

// #define toString(num) #num

// #define autSV 100
// #define autS task::sleep(autSV);

// Logical operators macros
// #define and &&
// #define or ||
// #define not !
// #define is ==
// #define isnot !=

//#define output_while(condition) while(condition){ \
//    task::sleep(10);
//
// #define END }

#define CHAIN return *this;
//
//     #define DRIVE_MECHANUM \
//double Y1 = abs(Greg.Axis3.value()) > sensitivity? Greg.Axis3.value() : 0;  \
//double Y2 = abs(Greg.Axis2.value()) > sensitivity? Greg.Axis2.value() : 0;  \
//double X1 = abs(Greg.Axis4.value()) > sensitivity? Greg.Axis4.value() : 0;  \
//double X2 = abs(Greg.Axis1.value()) > sensitivity? Greg.Axis1.value() : 0;  \
//double FLS = Y1 + X1;  \
//double BLS = Y1-X1;  \
//double FRS = Y2-X2;  \
//double BRS = Y2+X2;  \
//if((FLS != 0 || BLS != 0 || BRS != 0 || FRS != 0)){    \
//wheelSpin(BLS, BRS, FLS, FRS);    \
//wheelsMove++;   \
//}   \
//else if (wheelsMove > 0) {     \
//wheelSpin(0, 0, 0, 0);    \
//    wheelsMove = 0;     \
//}       \
//;
//
//     #define DRIVE_MECHANUM2 \
//double Y1 = abs(Greg.Axis3.value()) > sensitivity? Greg.Axis3.value() : 0;  \
//double Y2 = abs(Greg.Axis2.value()) > sensitivity? Greg.Axis2.value() : 0;  \
//double X1 = abs(Greg.Axis4.value()) > sensitivity? Greg.Axis4.value() : 0;  \
//double X2 = abs(Greg.Axis1.value()) > sensitivity? Greg.Axis1.value() : 0;  \
//double FLS = Y1 + X1;  \
//double BLS = Y1-X1;  \
//double FRS = Y2-X2;  \
//double BRS = Y2+X2;  \
//if((FLS != 0 || BLS != 0 || BRS != 0 || FRS != 0)){    \
//BL.spin(directionType::fwd, BLS, velocityUnits::pct);\
//BR.spin(directionType::fwd, BRS, velocityUnits::pct);\
//FL.spin(directionType::fwd, FLS, velocityUnits::pct);\
//FR.spin(directionType::fwd, FRS, velocityUnits::pct);\
//auton_write("BL.spin(directionType::fwd, ", parseInt(BLS) + ", velocityUnits::pct);");\
//auton_write("BR.spin(directionType::fwd, ", parseInt(BRS) + ", velocityUnits::pct);");\
//auton_write("FL.spin(directionType::fwd, ", parseInt(FLS) + ", velocityUnits::pct);");\
//auton_write("FR.spin(directionType::fwd, ", parseInt(FRS) + ", velocityUnits::pct);");\
//wheelsMove++;   \
//}   \
//else if (wheelsMove > 0) {     \
//BL.spin(directionType::fwd, 0, velocityUnits::pct);\
//BR.spin(directionType::fwd, 0, velocityUnits::pct);\
//FL.spin(directionType::fwd, 0, velocityUnits::pct);\
//FR.spin(directionType::fwd, 0, velocityUnits::pct);\
//auton_write("BL.spin(directionType::fwd, 0, velocityUnits::pct);\nBR.spin(directionType::fwd, 0, velocityUnits::pct);\nFL.spin(directionType::fwd, 0, velocityUnits::pct);\nFR.spin(directionType::fwd, 0, velocityUnits::pct); ");\
//    wheelsMove = 0;     \
//}       \
//;
//
//     #define DRIVE_OMNI2 \
//double Y1 = abs(Greg.Axis3.value()) > sensitivity? Greg.Axis3.value() : 0;  \
//double Y2 = abs(Greg.Axis2.value()) > sensitivity? Greg.Axis2.value() : 0;  \
//double X1 = abs(Greg.Axis4.value()) > sensitivity? Greg.Axis4.value() : 0;  \
//double X2 = abs(Greg.Axis1.value()) > sensitivity? Greg.Axis1.value() : 0;  \
//double LS = Y1;  \
//double RS = Y2;  \
//BL.spin(directionType::fwd, LS, pct);                   \
//BR.spin(directionType::fwd, RS, pct);                   \
//;
//
//
//     #define DRIVE_OMNI \
//double Y1 = abs(Greg.Axis3.value()) > sensitivity? Greg.Axis3.value() : 0;  \
//double Y2 = abs(Greg.Axis2.value()) > sensitivity? Greg.Axis2.value() : 0;  \
//if(Y1 != 0 || Y2 != 0){    \
//wheelSpin(Y1, Y2);    \
//wheelsMove++;   \
//}   \
//else if (wheelsMove > 0) {     \
//wheelSpin(0, 0);    \
//    wheelsMove = 0;     \
//}       \
//;
//
// #define randomSeed timer::systemHighResolution()
#endif

#ifdef MAKE
void s(int time) {
    task::sleep(time);
}
template <typename Number>
string parseInt(Number n) {

    char str[20];
    if (n < 0) {
        string ret = "-";
        ret += parseInt(abs(n));
        return ret;
    }
    string buf = itoa(n, str, 10);
    return buf;
}

template <>
string parseInt<string>(string n) {
    return n;
}
template <>
string parseInt<const char*>(const char* n) {
    return n;
}
#else
void s(int);
template <typename Number>
string parseInt(Number n);
template <>
string parseInt<string>(string n);
template <>
string parseInt<const char*>(const char* n);
#endif
