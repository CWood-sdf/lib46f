#include "stuff.h"
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
    bool call(bool remake = false);

public:
    /**
     * @brief Adds a new function to the list
     *
     * @param fn the function to be added
     */
    static void addNewFn(BosFn fn);
    /**
     * @brief Sets the screen switch buttons to be transparent
     *
     */
    static void useTransparentScreenSwitchButtons();
    BosFn() = delete;
    /**
     * @brief Construct a new Bos Fn object
     *
     * @param fn the function to be called
     */
    BosFn(bool (*fn)(bool));
    /**
     * @brief Construct a new Bos Fn object
     *
     * @param fn the function to be called
     */
    BosFn(void (*fn)(bool));
    /**
     * @brief Compares the function pointers of two BosFn objects
     *
     * @param other the other BosFn object
     * @return true if the function pointers are the same
     * @return false if the function pointers are different
     */
    bool operator==(const BosFn& other) const;
    /**
     * @brief An infinite loop that runs the brainOS
     *
     */
    static void runBrainOS();
};
