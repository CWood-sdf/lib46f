// vex.h -- This file includes all external files, defines random functions
//       and defines devices
#ifndef VEX_H
#define VEX_H
// #define BASE_FILE <C:/Users/Student/Documents/GitHub/Libs
// #define BASE_FILE <C:/Users/woodc/OneDrive/GitHub/Libs-Actual/Libs
// clang-format off
#define BASE_FILE <Libs

// #undef USE_GAME_SPECIFIC
//
// #define TEST
// Disable all warnings that exist bc I like using C++17 stuff
#pragma GCC diagnostic ignored "-Wc++17-extensions"
#pragma GCC diagnostic ignored "-Wunknown-attributes"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
// #pragma GCC diagnostic ignored "-Wunused-variable"
#define USE_MATH_DEFINES
// Include a few basic files
#include <list>

#include <math.h>
// #include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

// #define ADD_THIS instances.push_back(this)

// Eliminate vex:: and std::
using namespace vex;
using namespace std;

#define V5_SENSOR_REFRESH 10 /*msec*/

#include "lvgl_inc.h"
// Include my epicly epic background files
// clang-format off
#define BASE BASE_FILE
#include BASE_FILE/basefile.h>
/*<>*/

//Preprocessor trig functions using taylor series
#define _sin(t) (t - t*t*t/6 + t*t*t*t*t/120 - t*t*t*t*t*t*t/5040 + t*t*t*t*t*t*t*t*t/362880)
#define _cos(t) (1 - t*t/2 + t*t*t*t/24 - t*t*t*t*t*t/720 + t*t*t*t*t*t*t*t/40320)
#define _tan(t) (_sin(t) / _cos(t))


ostream& operator<<(ostream& cout, PVector& v);
ostream& operator<<(ostream& cout, PVector&& v);

// This is necessary because, due to C++ rounding errors, 0.3 * 10 could be 2.9999999
int mostlyFloor(double v);
// Converts an int to a string
string toCcp(int v);
// Converts double to string
string toCcp(double v);
#endif
