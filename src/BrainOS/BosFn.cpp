#include "BrainOS/BosFn.h"
#define V5_LVGL_RATE 4
void vexTaskSleep(uint32_t time);

bool BosFn::call(bool remake) {
    if (isPop) {
        return popFn(remake);
    } else {
        mainFn(remake);
    }
    return false;
}
/**
 * @brief Adds a new function to the list
 *
 * @param fn the function to be added
 */
void BosFn::addNewFn(BosFn fn) {
    bosFns.pushBack(fn);
}
/**
 * @brief Sets the screen switch buttons to be transparent
 *
 */
void BosFn::useTransparentScreenSwitchButtons() {
    transparentScreenSwitch = true;
}
/**
 * @brief Construct a new Bos Fn object
 *
 * @param fn the function to be called
 */
BosFn::BosFn(bool (*fn)(bool)) {
    popFn = fn;
    isPop = true;
}
/**
 * @brief Construct a new Bos Fn object
 *
 * @param fn the function to be called
 */
BosFn::BosFn(void (*fn)(bool)) {
    mainFn = fn;
    isPop = false;
}
/**
 * @brief Compares the function pointers of two BosFn objects
 *
 * @param other the other BosFn object
 * @return true if the function pointers are the same
 * @return false if the function pointers are different
 */
bool BosFn::operator==(const BosFn& other) const {
    return isPop ? popFn == other.popFn : mainFn == other.mainFn;
}
/**
 * @brief An infinite loop that runs the brainOS
 *
 */
void BosFn::runBrainOS() {
    cout << "Start brainOS" << endl;
    int32_t col = 0x0a323232;
    if (!transparentScreenSwitch) {
        col = 0xff323232;
    }
    // Set it to 50 gray and 10 transparent
    color buttonColorPress = color(col);
    color buttonColor = color(col);

    Button screenLeft = Button(/*Brain, */ 0, BRAIN_HEIGHT - 60, 40, 40, buttonColor, buttonColorPress, "<", -40, -30);
    Button screenRight = Button(/*Brain, */ BRAIN_WIDTH - 40, BRAIN_HEIGHT - 60, 40, 40, buttonColor, buttonColorPress, ">", -40, -30);
    int emptyCount = 0;
    while (bosFns.empty()) {
        cout << "bosFns is empty for some reason" << endl;
        s(500);
        if (++emptyCount == 10) {
            cout << "bosFns is empty for 5 seconds, exiting" << endl;
            return;
        }
    }
    bosFns.getCurrent()->call(true);
    while (1) {
        // Have buttons clicked first so that clicking them overrides the screen click functions
        if (screenLeft.clicked() && &bosFns.getBase() != &bosFns.getCurrent()) {
            // BUG: (maybe) I'm not sure if this will crash anything if the screen is not lvgl, it should be fine though
            //  // If it's lvgl, clean it
            //  if (bosFns.getCurrent()->lvgl())
            //  {
            cout << "Clean" << endl;
            // Remove all objects
            lv_obj_clean(lv_scr_act());
            lv_anim_del_all();
            // }
            bosFns.moveCurrentLeft();
            // Tell it to remake
            bosFns.getCurrent()->call(true);
        } else if (screenRight.clicked() && &bosFns.getEnd() != &bosFns.getCurrent()) {
            // if (bosFns.getCurrent()->lvgl())
            // {
            cout << "Clean" << endl;
            // Remove all objects
            lv_obj_clean(lv_scr_act());
            lv_anim_del_all();
            // }
            // Shift the linked list pointer
            bosFns.moveCurrentRight();
            // Tell it to remake
            bosFns.getCurrent()->call(true);
        }
        // Draw the screen, and store it's pop result
        auto result = bosFns.getCurrent()->call(false);
        // BUG: (maybe) I'm not sure if this will crash anything if the screen is not lvgl, it should be fine though
        //  if (bosFns.getCurrent()->lvgl())
        //  {
        lv_tick_inc(V5_LVGL_RATE);
        lv_task_handler();
        // }
        // If we should pop the element from the list
        if (result) {
            // BUG:
            //  if (bosFns.getCurrent()->lvgl())
            //  {
            cout << "Clean" << endl;
            // Remove all objects
            lv_obj_clean(lv_scr_act());
            lv_anim_del_all();
            // }
            bosFns.popCurrent();
            // Send it the remake command
            bosFns.getCurrent()->call(true);
        }
        // Draw the buttons
        // screenLeft.draw();
        // screenRight.draw();

        // Allow other tasks to run
        vex::task::sleep(V5_LVGL_RATE);
        // Wait for the screen to refresh, just in case the wait wasn't long enough
        vexDisplayRender(true, true);
    }
}
