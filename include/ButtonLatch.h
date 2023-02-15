#ifndef BUTTONLATCH_H
#define BUTTONLATCH_H
#include "vex.h"
class ButtonLatch
{
    // Is true when the button is held down
    bool isPressing = false;
    // The current state number of the latch
    int state = 0;
    // The maximum value of the state
    int stateLim;
    // The button to latch to
    const controller::button& b;

public:
    /**
     * @brief Construct a new Button Latch object
     *
     * @param b the button to latch to
     * @param stateLim The number of states, defaults to 2 for pneumatics
     */
    ButtonLatch(const controller::button& b, int stateLim = 2) : stateLim(stateLim), b(b)
    {
    }
    ButtonLatch() = delete;
    /**
     * @brief Returns true when the button is first pressed
     * @attention Designed to be used directly in the while loop like everything else
     *
     * @return true The button is pressed
     * @return false
     */
    bool pressing()
    {
        if (b.pressing() && !isPressing)
        {
            state++;
            if (state == stateLim + 1)
            {
                state = 1;
            }

            isPressing = true;
            return true;
        }
        else if (!b.pressing())
        {
            isPressing = false;
        }
        return false;
    }
    /**
     * @brief Get the latch state
     *
     * @return int The state
     */
    int getState()
    {
        return state;
    }
};
#endif
