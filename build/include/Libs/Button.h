#pragma once
#define BRAIN_WIDTH 480
#define BRAIN_HEIGHT 220
#include <string>
namespace vex {
    class brain;
    class color;
}
struct Button {
protected:
    // vex::brain& Brain;
    int x;
    int y;
    int width;
    int height;
    string label;
    vex::color fill = vex::color(0, 0, 0);
    vex::color fill2 = vex::color(50, 50, 50);
    int xOff = 0;
    int yOff = 0;
    bool pressedLast = false;

public:
    /**
     * @brief Construct a new Button object
     *
     * @param Brain the brain screen to draw on
     * @param x the x position
     * @param y the y position
     * @param width the width
     * @param height the height
     * @param fill the fill color
     * @param label the label
     * @param xOff the x offset of the label
     * @param yOff the y offset of the label
     */
    explicit Button(/*vex::brain& Brain, */ int x, int y, int width, int height, vex::color fill, string label, int xOff = 0, int yOff = 0)
      : // Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        xOff(xOff),
        yOff(yOff) {
        this->fill = fill.rgb();
        fill2 = fill.rgb();
    }
    /**
     * @brief Construct a new Button object
     *
     * @param Brain the brain screen to draw on
     * @param x the x position
     * @param y the y position
     * @param width the width
     * @param height the height
     * @param fill the fill color
     * @param fill2 the fill color when pressed
     * @param label the label
     * @param xOff the x offset of the label
     * @param yOff the y offset of the label
     */
    explicit Button(/*vex::brain& Brain, */ int x, int y, int width, int height, vex::color fill, vex::color fill2, string label, int xOff = 0, int yOff = 0)
      : // Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        xOff(xOff),
        yOff(yOff) {
        this->fill = fill.rgb();
        this->fill2 = fill2.rgb();
    }
    void draw() {
        // fill(this.fill);
        // Brain.Screen.setFillColor(this->fill);
        // if (pressing())
        // {
        //     Brain.Screen.setFillColor(this->fill2);
        // }
        // Brain.Screen.drawRectangle(x, y, width, height);
        if (pressing()) {
            vexDisplayForegroundColor(this->fill2);
        } else {
            vexDisplayForegroundColor(this->fill);
        }
        vexDisplayRectFill(x, y, width + x, height + y);

        vexDisplayStringAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
        // Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
    }
    bool pressing() {
        V5_TouchStatus status;
        vexTouchDataGet(&status);
        int mouseX = status.lastXpos;
        int mouseY = status.lastYpos;
        if (status.lastEvent == kTouchEventPress && mouseX > x &&
            mouseX < x + width &&
            mouseY > y &&
            mouseY < y + height) {

            return true;
        }
        return false;
    }
    bool clicked() {
        if (pressing()) {

            V5_TouchStatus status;
            while (status.lastEvent == kTouchEventPress) {
                vexTouchDataGet(&status);
                draw();
                task::sleep(10);
            }

            return true;
        }
        return false;
    }
    bool released() {
        // Return true when pressing() is false but lastPressed is true
        bool p = pressing();
        if (!p && pressedLast) {
            pressedLast = false;
            return true;
        }
        pressedLast = p;
        return false;
    }
    bool pressed() {
        // Return true when pressing() is true but lastPressed is false
        bool p = pressing();
        if (p && !pressedLast) {
            pressedLast = true;
            return true;
        }
        pressedLast = p;
        return false;
    }
};

// struct MovingButton : public Button
// {
// private:
//     bool clickedLast = false;

// public:
//     explicit MovingButton(vex::brain& Brain, int x, int y, int width, int height, vex::color fill, string label)
//       : Button(/*Brain, */ x, y, width, height, fill, label)
//     {
//     }
//     explicit MovingButton(vex::brain& Brain, int x, int y, int width, int height, vex::color fill, string label, int xOff, int yOff)
//       : Button(/*Brain, */ x, y, width, height, fill, label, xOff, yOff)
//     {
//     }
//     explicit MovingButton(vex::brain& Brain, int x, int y, int width, int height, vex::color fill, vex::color fill2, string label, int xOff, int yOff)
//       : Button(/*Brain, */ x, y, width, height, fill, fill2, label, xOff, yOff)
//     {
//     }
//     void draw()
//     {
//         // fill(this.fill);
//         bool p = pressing();
//         Brain.Screen.setFillColor(p ? this->fill2 : this->fill);
//         Brain.Screen.drawRectangle(x, y, width, height);

//         Brain.Screen.setCursor(2, 1);
//         Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
//         if (p || (clickedLast && Brain.Screen.pressing()))
//         {
//             x = Brain.Screen.xPosition();
//             y = Brain.Screen.yPosition();
//         }
//         clickedLast = p;
//     }
// };
