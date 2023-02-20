#include "ButtonLatch.h"
/**
 * @brief Construct a new Button Latch object
 *
 * @param b the button to latch to
 * @param stateLim The number of states, defaults to 2 for pneumatics
 */
ButtonLatch::ButtonLatch(const controller::button& b, int stateLim) : stateLim(stateLim), b(b) {
}
/**
 * @brief Returns true when the button is first pressed
 * @attention Designed to be used directly in the while loop like everything else
 *
 * @return true The button is pressed
 * @return false
 */
bool ButtonLatch::pressing() {
    if (b.pressing() && !isPressing) {
        state++;
        if (state == stateLim + 1) {
            state = 1;
        }

        isPressing = true;
        return true;
    } else if (!b.pressing()) {
        isPressing = false;
    }
    return false;
}
/**
 * @brief Get the latch state
 *
 * @return int The state
 */
int ButtonLatch::getState() {
    return state;
}
