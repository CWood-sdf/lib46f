#include "Odometry/FieldCoord.h"

FieldCoord::FieldCoord()
{
    pos = PVector();
    angle = 0.0;
}
FieldCoord::FieldCoord(PVector& v, double& a)
{
    pos = v;
    angle = a;
}
FieldCoord::FieldCoord(PVector&& v, double&& a)
{
    pos = v;
    angle = a;
}
FieldCoord& FieldCoord::set(PVector v, double a)
{
    pos = v;
    angle = a;
    CHAIN
}
bool FieldCoord::operator!=(FieldCoord& f)
{
    return f.pos != pos || f.angle != angle;
}
bool FieldCoord::operator==(FieldCoord& f)
{
    return f.pos == pos && f.angle == angle;
}
FieldCoord& FieldCoord::operator+=(FieldCoord& f)
{
    this->pos += f.pos;
    this->angle += f.angle;
    CHAIN
}
FieldCoord FieldCoord::operator-(FieldCoord& p)
{
    return FieldCoord(pos - p.pos, angle - p.angle);
}
FieldCoord FieldCoord::operator-()
{
    return FieldCoord(-pos, angle + 180);
}
FieldCoord FieldCoord::operator+(FieldCoord p)
{
    return FieldCoord(pos + p.pos, angle + p.angle);
}
FieldCoord FieldCoord::operator*(double d)
{
    return FieldCoord(pos * d, angle * d);
}
typedef FieldCoord Pose;
ostream& operator<<(ostream& cout, FieldCoord& v)
{
    cout << VECT_OUT(v.pos) << ", " << v.angle;
    return cout;
}
ostream& operator<<(ostream& cout, FieldCoord&& v)
{
    return operator<<(cout, v);
}
