#include "vex.h"
#define V5_LVGL_RATE 4
void vexTaskSleep(uint32_t time);
class BosFn
{
    bool (*popFn)(bool);
    void (*mainFn)(bool);
    bool isPop = false;
    static inline LinkedList<BosFn> bosFns = LinkedList<BosFn>();
    static inline bool transparentScreenSwitch = false;
    /**
     * @brief Calls the function
     *
     * @param remake
     * @return true
     * @return false
     */
    bool call(bool remake = false)
    {
        if (isPop)
        {
            return popFn(remake);
        }
        else
        {
            mainFn(remake);
        }
        return false;
    }

public:
    /**
     * @brief Adds a new function to the list
     *
     * @param fn the function to be added
     */
    static void addNewFn(BosFn fn)
    {
        bosFns.pushBack(fn);
    }
    /**
     * @brief Sets the screen switch buttons to be transparent
     *
     */
    static void useTransparentScreenSwitchButtons()
    {
        transparentScreenSwitch = true;
    }
    BosFn() = delete;
    /**
     * @brief Construct a new Bos Fn object
     *
     * @param fn the function to be called
     */
    BosFn(bool (*fn)(bool))
    {
        popFn = fn;
        isPop = true;
    }
    /**
     * @brief Construct a new Bos Fn object
     *
     * @param fn the function to be called
     */
    BosFn(void (*fn)(bool))
    {
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
    bool operator==(const BosFn& other) const
    {
        return isPop ? popFn == other.popFn : mainFn == other.mainFn;
    }
    /**
     * @brief An infinite loop that runs the brainOS
     *
     */
    static void runBrainOS()
    {
        cout << "Start brainOS" << endl;

        // Set it to 50 gray and 10 transparent
        color buttonColorPress = color(0x0a3232ff);
        color buttonColor = color(0x0a323232);
        if (transparentScreenSwitch)
        {
            // Set the transparency to true
            // HACK:
            *((bool*)(((uint32_t*)&buttonColor) + 1)) = true;
        }
        Button screenLeft = Button(Brain, 0, BRAIN_HEIGHT - 60, 40, 40, buttonColor, buttonColorPress, "<", -40, -30);
        Button screenRight = Button(Brain, BRAIN_WIDTH - 40, BRAIN_HEIGHT - 60, 40, 40, buttonColor, buttonColorPress, ">", -40, -30);
        int emptyCount = 0;
        while (bosFns.empty())
        {
            cout << "bosFns is empty for some reason" << endl;
            s(500);
            if (++emptyCount == 10)
            {
                cout << "bosFns is empty for 5 seconds, exiting" << endl;
                return;
            }
        }
        bosFns.getCurrent()->call(true);
        while (1)
        {
            // Have buttons clicked first so that clicking them overrides the screen click functions
            if (screenLeft.clicked() && &bosFns.getBase() != &bosFns.getCurrent())
            {
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
            }
            else if (screenRight.clicked() && &bosFns.getEnd() != &bosFns.getCurrent())
            {
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
            if (result)
            {
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
};
