#include "Sensors/Wrappers/Encoder.h"

// The constructor, only enable if the type has been mapped
template <class Sensor>
Encoder::Encoder(Sensor* e, decltype(&Sensor::resetPosition))
{
    cout << "Init encoder" << endl;
    getValue = [e](rotationUnits units)
    {
        return e->position(units);
    };
    resetter = [e]()
    {
        e->resetPosition();
    };
}
template <class Sensor>
Encoder::Encoder(Sensor* e, decltype(&Sensor::resetRotation))
{
    getValue = [e](rotationUnits units)
    {
        return e->position(units);
    };
    resetter = [e]()
    {
        e->resetRotation();
    };
}
template <class Sensor>
Encoder::Encoder(Sensor* e, std::function<void()> f)
{
    getValue = [e](rotationUnits units)
    {
        return e->position(units);
    };
    resetter = f;
}
// Constructor that just takes the functions
Encoder::Encoder(std::function<double(rotationUnits)> f, std::function<void()> r)
{
    getValue = f;
    resetter = r;
}
template <class Sensor>
Encoder::Encoder(Sensor& e) : Encoder(&e)
{
}
Encoder::Encoder()
{
    getValue = [](rotationUnits units)
    {
        return 0;
    };
    resetter = [](void) {

    };
}
// Make the position method
double Encoder::position(rotationUnits units)
{
    return getValue(units);
}
void Encoder::resetPosition()
{
    resetter();
}
Encoder& Encoder::operator=(const Encoder& other)
{
    getValue = other.getValue;
    resetter = other.resetter;
    return *this;
}
Encoder& Encoder::operator=(Encoder&& other)
{
    getValue = other.getValue;
    resetter = other.resetter;
    return *this;
}
