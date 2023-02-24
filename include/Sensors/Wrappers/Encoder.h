#ifndef ENCODER_H
#define ENCODER_H
#include "stuff.h"
#include "v5_apitypes.h"
#include "vex_device.h"
#include "vex_triport.h"
#include <functional>
// A simple encoder wrapper, the only functions are getPosition and resetPosition
class Encoder {
    // encoder sdfsd = encoder(Brain.ThreeWirePort.A);
    // const V5_DeviceType encoderType = V5_DeviceType::kDeviceTypeMotorSensor;
    std::function<double(rotationUnits)> getValue;
    std::function<void()> resetter;
    // static inline devices device = devices();
public:
    /**
     * @brief Construct a new Encoder object
     *
     * @tparam Sensor The sensor type
     * @param e The sensor
     */
    template <class Sensor>
    Encoder(Sensor& e, decltype(&Sensor::resetPosition) = nullptr) {
        cout << "Init encoder" << endl;
        getValue = [&e](rotationUnits units) {
            return e.position(units);
        };
        resetter = [&e]() {
            e.resetPosition();
        };
    }
    /**
     * @brief Construct a new Encoder object
     *
     * @tparam Sensor The sensor type
     * @param e The sensor
     */
    template <class Sensor>
    Encoder(Sensor& e, decltype(&Sensor::resetRotation) = nullptr) {
        getValue = [&e](rotationUnits units) {
            return e.position(units);
        };
        resetter = [&e]() {
            e.resetRotation();
        };
    }
    /**
     * @brief Construct a new Encoder object
     *
     * @param f The position() function
     * @param r The resetPosition() function
     */
    Encoder(std::function<double(rotationUnits)> f, std::function<void()> r);
    /**
     * @brief Construct a new Encoder object
     *
     */
    Encoder();
    /**
     * @brief Returns the position of the encoder
     *
     * @param units The rotation units to return the position in
     * @return double
     */
    double position(rotationUnits units);
    /**
     * @brief Resets the position of the encoder
     *
     */
    void resetPosition();
    Encoder& operator=(const Encoder& other);
    Encoder& operator=(Encoder&& other);
};

#endif // ENCODER_H
