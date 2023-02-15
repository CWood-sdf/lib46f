#include "fns.h"
// ClassBaseFns.h -- use this file for all auton functions
#ifdef MAKE
list<function<void()>> threadFns;
#else
extern list<function<void()>> threadFns;
#endif

#define toStr(a) #a

// All the functions that are based on classes
//  namespace ClassFns {
void spinRoller();
// }
namespace ClassFns
{
}
