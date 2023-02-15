#include <vector>
#define nmdefaultRot rotationUnits::rev
#define vfor(it, v) \
    for (auto it : v)
#define vpfor(it, v) \
    for (auto* it : v)
#define vrfor(it, v) \
    for (auto& it : v)
#define vrep(it, v) \
    for (int it = 0; it < v.size(); it++)
#define prop(obj, name) \
    obj.name
#ifndef CHAIN
#define CHAIN return *this
#endif
#include <array>
#include <iostream>
#include <map>
#include <tuple>
namespace vex
{
    class pneumatics;
    class motor;
    enum class directionType;
    enum class percentUnits;
}
using namespace vex;
class MotorGroup;
class Pto
{
    friend class MotorGroup;
    int index = 0;
    operator int()
    {
        return index;
    }

public:
    Pto(int i) : index(i) {}
};
class MotorGroup
{
    int size = 0;
    static inline std::map<motor*, std::vector<MotorGroup*>> ptoMotors;
    typedef motor motor_type;
    typedef MotorGroup& chain_method;
    std::vector<double> lastVoltCmd = {};
    std::vector<motor_type*> m = std::vector<motor_type*>();
    std::vector<std::tuple<bool, std::vector<pneumatics*>, std::vector<int>>> pto = {};
    // might as well overallocate, this is just temporary anyway
    std::vector<bool> allowedMotors = std::vector<bool>(10, true);
    template <typename... args>
    void addMotor(args*... a)
    {
        m = {a...};
        size = m.size();
        lastVoltCmd = std::vector<double>(size, 0.0);
    }
    void setPtoAllowed()
    {
        // Fill allowedMotors with true
        allowedMotors = std::vector<bool>(m.size(), true);
        // Loop through pto and if first pneumatic matches the bool, fill allowedMotors indexes with false in allowedMotors
        for (auto& t : pto)
        {
            if (std::get<0>(t) == std::get<1>(t)[0]->value())
            {
                std::cout << "Pto is active " << this << std::endl;
                for (int i : std::get<2>(t))
                {
                    allowedMotors[i] = false;
                }
            }
        }
    }
    void reinvokeLast()
    {
        std::cout << "Reinvoking pto " << this << std::endl;
        setPtoAllowed();
        for (int i = 0; i < size; i++)
        {
            if (allowedMotors[i])
            {
                m[i]->spin(directionType::fwd, lastVoltCmd[i], percentUnits::pct);
            }
        }
    }
    /**
     * @brief Returns the index of the motor in the motor group
     *
     * @param m the motor to find
     * @return int the index of the motor, -1 if not found
     */
    int indexOf(motor_type* m)
    {
        for (int i = 0; i < size; i++)
        {
            if (m == this->m[i])
            {
                return i;
            }
        }
        return -1;
    }

public:
    template <typename... args>
    MotorGroup(args*... a)
    {
        addMotor(a...);
    }
    template <typename... args>
    MotorGroup(args&... a)
    {
        addMotor(&a...);
    }
    MotorGroup()
    {
        m = std::vector<motor_type*>();
        lastVoltCmd = std::vector<double>();
    }
    MotorGroup(const MotorGroup&) = delete;
    MotorGroup(MotorGroup&&) = delete;
    /**
     * @brief Adds a pto on the given motors
     *
     * @param p the pneumatics
     * @param motors the motors to run
     * @param desiredState runs the motors if the pneumatics are in this state
     * @return int the index of the PTO, store it in a Pto object
     */
    Pto addPto(pneumatics& p, std::vector<int> motors, bool desiredState)
    {
        for (int i : motors)
        {
            if (i >= m.size())
            {
                std::cerr << "MotorGroup::addPto: Motor index out of range" << std::endl;
                return -1;
            }
        }
        // Loop through motors and add all motors to ptoMotors
        for (int i : motors)
        {
            // Check if ptoMotors does not contain the motor
            if (ptoMotors.count(m[i]) == 0)
            {
                // Add the motor to ptoMotors
                ptoMotors[m[i]] = std::vector<MotorGroup*>();
            }
            ptoMotors[m[i]].push_back(this);
        }
        pto.push_back(make_tuple(desiredState, std::vector<pneumatics*>{&p}, motors));
        return pto.size() - 1;
    }
    /**
     * @brief Adds a pto on the given motors
     *
     * @param p the pneumatics
     * @param motors the motors to run
     * @param desiredState runs the motors if the pneumatics are in this state
     * @return int the index of the PTO, store it in a Pto object
     */
    Pto addPto(pneumatics& p, std::vector<motor*> motors, bool desiredState)
    {
        std::vector<int> motorIndexes = {};
        for (motor* mot : motors)
        {
            int index = indexOf(mot);
            if (index < 0)
            {
                std::cerr << "MotorGroup::addPto: Motor not found" << std::endl;
                return -1;
            }
            motorIndexes.push_back(index);
        }
        return addPto(p, motorIndexes, desiredState);
    }
    /**
     * @brief Sets the given pto to the drive state and respins the motors
     *
     * @param ptoIndex the index of the pto
     * @return chain_method
     */
    chain_method setPtoDrive(Pto ptoToUse)
    {
        int ptoIndex = ptoToUse;
        if (ptoIndex >= pto.size() || ptoIndex < 0)
        {
            std::cerr << "MotorGroup::setPtoDrive: PTO index out of range" << std::endl;
            CHAIN;
        }
        // Loop through desired pto and set the pneumatics to the desired state
        for (auto& p : std::get<1>(pto[ptoIndex]))
        {
            p->set(std::get<0>(pto[ptoIndex]));
        }
        reinvokeLast();
        CHAIN;
    }
    /**
     * @brief Sets the given pto to the release state and respins the motors attached on the other end of the pto
     *
     * @param ptoIndex the index of the pto
     * @return chain_method
     */
    chain_method setPtoRelease(Pto ptoToUse)
    {
        int ptoIndex = ptoToUse;
        if (ptoIndex >= pto.size() || ptoIndex < 0)
        {
            std::cerr << "MotorGroup::setPtoRelease: PTO index out of range" << std::endl;
            CHAIN;
        }
        // Loop through desired pto and set the pneumatics to the opposite of the desired state
        for (auto& p : std::get<1>(pto[ptoIndex]))
        {
            p->set(!std::get<0>(pto[ptoIndex]));
        }
        // Get the motors that are not allowed to be driven
        std::vector<int> notAllowed = std::get<2>(pto[ptoIndex]);
        // Loop through notAllowed and set the map pointers to reinvoke
        for (int i : notAllowed)
        {
            // Loop through the pointers in the map
            for (auto* n : ptoMotors[m[i]])
            {
                // If the pointer is not this, reinvoke
                if (n != this)
                {
                    n->reinvokeLast();
                    // Don't want a multiple reinvoke
                    break;
                }
            }
        }
        CHAIN;
    }
    /**
     * @brief Gets the specified motor from the index
     *
     * @param n the index of the motor
     * @return motor_type&
     */
    motor_type& operator[](int n)
    {
        return *m[n];
    }
    /**
     * @brief stops all the motors with their default brake type
     *
     */
    void stop()
    {
        setPtoAllowed();
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i++])
                n->stop();
        }
        lastVoltCmd = std::vector<double>(size, 0.0);
    }
    /**
     * @brief Spins the motors with the given direction and velocity
     *
     * @param dir the direction to spin the motors
     * @param velocity the velocity to spin the motors
     * @param v the units of the velocity
     * @return chain_method
     */
    chain_method spin(directionType dir, double velocity, percentUnits v = pct)
    {
        setPtoAllowed();
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i++])
            {
                n->spin(dir, velocity, v);
            }
        }
        lastVoltCmd = std::vector<double>(size, velocity * (v == pct ? 1.0 : 120.0 / 100.0) * (dir == fwd ? 1.0 : -1.0));
        return *this;
    }
    /**
     * @brief Spins the motors at the specified percent velocity but on voltage units
     *
     * @param dir the direction to spin the motors
     * @param velocityPct the percent velocity to spin the motors
     * @return chain_method
     */
    chain_method spinVolt(directionType dir, int velocityPct)
    {
        setPtoAllowed();
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i])
            {
                n->spin(dir, velocityPct * 0.12, volt);
            }
            i++;
        }
        lastVoltCmd = std::vector<double>(size, velocityPct * 0.12 * (dir == fwd ? 1.0 : -1.0));
        return *this;
    }
    /**
     * @brief Spins the motors at the specified percent velocities
     *
     * @param speeds The speeds to spin the motors at
     * @return chain_method
     */
    chain_method seperateSpin(std::vector<double> speedsPct)
    {
        setPtoAllowed();
        if (speedsPct.size() != m.size())
        {
            std::cerr << "MotorGroup::seperateSpin: Speeds vector size does not match motor size" << std::endl;
            return *this;
        }
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i])
            {
                n->spin(fwd, speedsPct[i], pct);
            }
            lastVoltCmd[i] = (double)speedsPct[i] * 0.12;
            i++;
        }

        return *this;
    }
    /**
     * @brief Spins the motors at the specified percent velocities
     *
     * @param speeds The speeds to spin the motors at
     * @return chain_method
     */
    chain_method seperateSpinVolt(std::vector<double> speedsPct)
    {
        setPtoAllowed();
        if (speedsPct.size() != m.size())
        {
            std::cerr << "MotorGroup::seperateSpinVolt: Speeds vector size does not match motor size" << std::endl;
            return *this;
        }
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i])
            {
                n->spin(fwd, speedsPct[i] * 0.12, volt);
            }
            lastVoltCmd[i] = (double)speedsPct[i] * 0.12;
            i++;
        }

        return *this;
    }

    // stop all motors
    chain_method stop(brakeType brak)
    {
        setPtoAllowed();
        int i = 0;
        for (auto n : m)
        {
            if (allowedMotors[i])
            {
                n->stop(brak);
            }
        }
        lastVoltCmd = std::vector<double>(size, 0.0);
        return *this;
    }
    void setBrakeMode(brakeType b)
    {
        for (auto n : m)
        {
            n->setStopping(b);
        }
    }
};
