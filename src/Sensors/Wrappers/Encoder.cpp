#include "Sensors/Wrappers/Encoder.h"

// Constructor that just takes the functions
Encoder::Encoder(std::function<double(rotationUnits)> f, std::function<void()> r)
{
    getValue = f;
    resetter = r;
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
