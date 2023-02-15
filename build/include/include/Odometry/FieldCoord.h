#include "vex.h"
struct FieldCoord
{
    PVector pos = PVector();
    double angle = 0.0;

public:
    FieldCoord();
    FieldCoord(PVector& v, double& a);
    FieldCoord(PVector&& v, double&& a);
    FieldCoord& set(PVector v, double a);
    bool operator!=(FieldCoord& f);
    bool operator==(FieldCoord& f);
    FieldCoord& operator+=(FieldCoord& f);
    FieldCoord operator-(FieldCoord& p);
    FieldCoord operator-();
    FieldCoord operator+(FieldCoord p);
    FieldCoord operator*(double d);
};
typedef FieldCoord Pose;
ostream& operator<<(ostream& cout, FieldCoord& v);
ostream& operator<<(ostream& cout, FieldCoord&& v);
