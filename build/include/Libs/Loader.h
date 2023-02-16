#pragma once

union PVector;
#ifdef MAKE
void windowsLoader(bool)
{
    static PVector pos = PVector(0.0, 0.0);
    static PVector velocity = PVector(1.5, 1.5);
    Brain.Screen.waitForRefresh();
    Brain.Screen.clearScreen(black);

    Brain.Screen.drawRectangle(pos.x, pos.y, 50, 50, red);
    Brain.Screen.drawRectangle(pos.x, pos.y + 50, 50, 50, blue);
    Brain.Screen.drawRectangle(pos.x + 50, pos.y, 50, 50, green);
    Brain.Screen.drawRectangle(pos.x + 50, pos.y + 50, 50, 50, yellow);
    /*Brain.Screen.drawCircle(pos.x, pos.y + 50, 50, yellow);
    Brain.Screen.drawRectangle(pos.x - 30, pos.y + 70, 60, 20, black);
    Brain.Screen.drawRectangle(pos.x - 30, pos.y + 30, 20, 20, black);
    Brain.Screen.drawRectangle(pos.x + 10, pos.y + 30, 20, 20, black);*/
    pos.add(velocity);
    if (pos.x + 100 > 480)
    {
        velocity.x *= -1;
    }
    if (pos.y + 100 > 240)
    {
        velocity.y *= -1;
    }
    if (pos.x < 0)
    {
        velocity.x *= -1;
    }
    if (pos.y < 0)
    {
        velocity.y *= -1;
    }
}
#else
void windowsLoader(bool);
#endif
