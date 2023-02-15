// vex.h -- This file includes all external files, defines random functions
//       and defines devices
#ifndef VEX_H
#define VEX_H
// #define BASE_FILE <C:/Users/Student/Documents/GitHub/Libs
// #define BASE_FILE <C:/Users/woodc/OneDrive/GitHub/Libs-Actual/Libs
// clang-format off
#define BASE_FILE <Libs
// clang-format on
// Undef this macro to find all places that are game specific
// #define USE_GAME_SPECIFIC
#define BOT 2
// #undef USE_GAME_SPECIFIC
//
// #define TEST
// Disable all warnings that exist bc I like using C++17 stuff
#pragma GCC diagnostic ignored "-Wc++17-extensions"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
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

// Make a brain
extern brain Brain;

// Make a controller and name it Greg
extern controller Greg;
extern controller Beethoven;

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

// clang-format on
#ifdef MAKE

ostream& operator<<(ostream& cout, PVector& v)
{
    cout << VECT_OUT(v);
    return cout;
}
ostream& operator<<(ostream& cout, PVector&& v)
{
    return operator<<(cout, v);
}

// This is necessary because, due to C++ rounding errors, 0.3 * 10 could be 2.9999999
int mostlyFloor(double v)
{
    // Get the decimal
    double decimals = v - floor(v);
    if (decimals > 0.99)
    {
        return round(v);
    }
    return floor(v);
}
// Converts an int to a string
string toCcp(int v)
{
    // The return value
    string ret;
    // Convert single digit integers to characters
    array<char, 10> arr = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    // bool neg = false;
    // Check if it's negative, if so add a "-" to ret
    if (v < 0)
    {
        // neg = true;
        v *= -1;
        ret = "-";
        ret += toCcp(v);
        return ret;
    }
    // While v is greater than 9
    // This loop converts it into a string backwards
    while (v > 9)
    {
        // Store a copy of v
        // For example: v = 42
        double copy = v;
        // divide the copy by 10
        // Ex: v = 4.2
        copy /= 10.0;
        // Store the last integer number of v
        // Ex: dif = mosltyFloor((4.2 - 4) * 10) [dif becomes 2]
        int dif = mostlyFloor((copy - floor(copy)) * 10.0);
        // Add the resulting character to ret
        // Ex: ret += '2';
        ret += arr[dif];
        // Store a floor of copy into v
        // Ex: v = 4
        v = floor(copy);
    }
    // Add the last integer to the array
    ret += arr[v > 9 ? 9 : v];

    // Reverse the string
    for (int i = 0; i < ret.size() / 2; i++)
    {
        // Swap the characters that are i distance from the front
        // And i distance from the back
        char retI = ret[i];
        ret[i] = ret[ret.size() - 1 - i];
        ret[ret.size() - 1 - i] = retI;
    }
    return ret;
}
// Converts double to string
string toCcp(double v)
{
    string ret;
    array<char, 10> arr = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    // bool neg = false;
    if (v < 0)
    {
        // neg = true;
        v *= -1.0;
        ret = "-";
        ret += toCcp(v);
        return ret;
    }
    double orgCopy = v;
    while (v > 9)
    {
        double copy = v;
        copy /= 10.0;
        int dif = (copy - floor(copy)) * 10;
        ret += arr[dif];
        v = floor(copy);
    }
    ret += arr[(int)(v > 9 ? 9 : v)];
    for (int i = 0; i < ret.size() / 2; i++)
    {
        char retI = ret[i];
        ret[i] = ret[ret.size() - 1 - i];
        ret[ret.size() - 1 - i] = retI;
    }
    v = orgCopy - floor(orgCopy);
    ret += ".";
    int i = 0;
    do
    {
        v *= 10.0;
        ret += arr[(int)v];
        v -= floor(v);
    } while (v != 0.0 && i++ < 5);
    return ret;
}
#else

ostream& operator<<(ostream& cout, PVector& v);
ostream& operator<<(ostream& cout, PVector&& v);

// This is necessary because, due to C++ rounding errors, 0.3 * 10 could be 2.9999999
int mostlyFloor(double v);
// Converts an int to a string
string toCcp(int v);
// Converts double to string
string toCcp(double v);
#endif
#endif
