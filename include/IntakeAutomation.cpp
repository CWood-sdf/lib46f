#include "IntakeAutomation.h"

#if BOT == 1
AutoIntake::AutoIntake(vector<std::function<bool()>> sensors)
{
    this->sensors = sensors;
}
void AutoIntake::makeMask()
{
    diskMask = 0;
    for (int i = 0; i < sensors.size(); i++)
    {
        diskMask |= (sensors[i]() << 2 * i);
    }
}
bool AutoIntake::stable()
{
    // Is stable if it does not end with 0's
    //  (i.e. return true if 0b011 or 0b001 or 0b111 or 0b000 but not 0b010 or 0b100)
    int lastBit = 1;
    for (int i = 0; i < sensors.size(); i++)
    {
        int bit = (diskMask >> i) & 1;
        if (bit == 1 && lastBit == 0)
        {
            return false;
        }
        lastBit = bit;
    }
    return true;
}
bool AutoIntake::fixableUnstable()
{
    // Is fixable if it ends with 0's
    //  (i.e. return true if 0b010 or 0b100 but not 0b011 or 0b001 or 0b111 or 0b000 or 0b101)
    int lastBit = 0;
    for (int i = 0; i < sensors.size(); i++)
    {
        int bit = (diskMask >> i) & 1;
        if (bit == 0 && lastBit == 1)
        {
            return false;
        }
        lastBit = bit;
    }
    return true;
}

void AutoIntake::disable()
{
    disabled = true;
}

void AutoIntake::enable()
{
    disabled = false;
}

void AutoIntake::updateValues(bool flywheelReady)
{
    this->flywheelReady = flywheelReady;
    static int i = 0;
    makeMask();
    // Mask correction
    if (diskMask != lastMask)
    {
        if ((lastMask & 0b10) == 0b10)
        {
            if (direction == 1 && (diskMask & 0b100) == 0b0)
            {
                diskMask |= 0b10;
            }
            if (direction == -1 && (diskMask & 0b1) == 0b0)
            {
                diskMask |= 0b10;
            }
        }
        if ((lastMask & 0b1000) == 0b1000)
        {
            if (direction == 1 && (diskMask & 0b10000) == 0b0)
            {
                diskMask |= 0b1000;
            }
            if (direction == -1 && (diskMask & 0b100) == 0b0)
            {
                diskMask |= 0b1000;
            }
        }
        if ((diskMask & 0b10000) == 0 && (lastMask & 0b10000) == 1)
        {
            if (direction == 1)
            {
                // Disk has exited
            }
            if (direction == -1)
            {
                // Move disk to interstage, only if there isn't a disk there
                if ((lastMask & 0b1000) == 0)
                {
                    // Move disk to interstage
                    diskMask |= 0b1000;
                }
                else
                {
                    // Leave disk up top
                    diskMask |= 0b10000;
                }
            }
        }
        if ((diskMask & 0b100) == 0 && (lastMask & 0b100) == 1)
        {
            if (direction == 1)
            {
                // Move disk to middle interstage, only if there isn't a disk there
                if ((lastMask & 0b1000) == 0)
                {
                    // Move disk to interstage
                    diskMask |= 0b1000;
                }
                else
                {
                    // Leave disk there
                    diskMask |= 0b100;
                }
            }
            if (direction == -1)
            {
                // Move disk to interstage, only if there isn't a disk there
                if ((lastMask & 0b10) == 0)
                {
                    // Move disk to interstage
                    diskMask |= 0b10;
                }
                else
                {
                    // Leave disk there
                    diskMask |= 0b100;
                }
            }
        }
        if ((diskMask & 0b1) == 0 && (lastMask & 0b1) == 1)
        {
            if (direction == 1)
            {
                // Move disk to bottom interstage, only if there isn't a disk there
                if ((lastMask & 0b10) == 0)
                {
                    // Move disk to interstage
                    diskMask |= 0b10;
                }
                else
                {
                    // Leave disk there
                    diskMask |= 0b1;
                }
            }
            if (direction == -1)
            {
                // Disk is out
            }
        }
    }
    lastMask = diskMask;
    count = 0;
    // Get the count of disks
    for (int i = 0; i < sensors.size() * 2 - 1; i++)
    {
        count += (diskMask >> i) & 1;
    }
    // If we're intaking, and a disk is at the top, then stop intaking
    if (intaking && sensors.back()())
    {
        intaking = false;
    }
    // If we're intaking, and we have the target count, then stop intaking
    if (intaking && count == targetCount)
    {
        intaking = false;
    }
    if (clearingDisks && count == 0)
    {
        clearingDisks = false;
        // clearingLastDisk = false;
    }
    // The mask for the last disk
    int lastDisk = 1 << (2 * sensors.size() - 2);
    // if (diskMask == lastDisk && clearingDisks)
    // {
    //   clearingLastDisk = true;
    // }
    // else
    // {
    //   clearingLastDisk = false;
    // }
    // If there is a disk in the last spot, then tempClearReady is true
    if ((diskMask & lastDisk) == lastDisk && clearingDisks)
    {
        tempClearReady = true;
    }
    else
    {
        tempClearReady = false;
    }
    // If the last disk is gone, then stop the intake for a little bit
    if (clearingDisks && tempClearReady && (diskMask & lastDisk) == 0)
    {
        tempClearStop = 200;
        tempClearReady = false;
    }
    if (tempClearStop > 0)
    {
        tempClearStop -= 10;
    }
    // If n seconds of clearing disks, then stop
    if (clearingDisks && (Brain.Timer.system() - clearStartTime) > 10000)
    {
        // clearingDisks = false;
    }
    // Direction is default stop
    direction = 0;
    // If it's intaking or
    //    clearing disks and
    //    either the flywheel is ready or there is no disk up top and
    //    it's not in a tempClearStop
    if (intaking || (clearingDisks && (flywheelReady || (!sensors.back()() && (diskMask & 0b1000) == 0)) && tempClearStop <= 0))
    {
        direction = 1;
    }
    // If it's not moving forward
    //    and it's not stable and it's fixable or it's fixing an unstable state
    //    and it's not clearing disks (aka it's not in a tempClearStop)
    if (direction != 1 && ((!stable() && fixableUnstable()) || fixingUnstable) && !clearingDisks)
    {
        direction = -1;
        fixingUnstable = true;
    }
    if (direction == 0 && clearingDisks && sensors.back()())
    {
        direction = -1;
    }
    if (i++ == 30)
    {
        // cout << "Mask: " << diskMask << endl;
        // cout << "Stable: " << stable() << endl;
        // cout << "Fixable: " << fixableUnstable() << endl;
        // cout << "Count: " << count << endl;
        // cout << "Direction: " << direction << endl;
        i = 0;
    }
    // If it's fixing an unstable state and either it's stable or there are no disks, then stop fixing
    if (fixingUnstable && (stable() || count == 0))
    {
        fixingUnstable = false;
    }
    lastCount = count;
}

bool AutoIntake::hasDisks()
{
    return count > 0;
}

void AutoIntake::setFiring()
{
    clearingDisks = true;
    clearStartTime = Brain.Timer.system();
}

void AutoIntake::intake()
{
    intaking = true;
    targetCount = count + 1;
}

void AutoIntake::intakeMultiple(int count)
{
    intake();
    // Override the target count
    targetCount = this->count + count;
    if (targetCount > 3)
    {
        targetCount = 3;
    }
}

bool AutoIntake::spinMotor()
{
    return direction == 1;
}

bool AutoIntake::reverseMotor()
{
    return direction == -1;
}

void AutoIntake::waitForFiring()
{
    // Wait for the firing to be done
    while (clearingDisks)
    {
        wait(10, msec);
    }
}

void AutoIntake::waitForIntake()
{
    // Wait for the intake to be done
    while (intaking)
    {
        wait(10, msec);
    }
}

void AutoIntake::autonInit()
{
    // This is to make preloads work
    fixingUnstable = false;
    clearingDisks = false;
    intaking = false;
    lastCount = count;
}
short operator"" _s(unsigned long long x)
{
    return static_cast<short>(x);
}
void AutoIntake::drawState(bool refresh)
{
    static vector<lv_obj_t*> lines = {};
    static lv_obj_t* upLine;
    static lv_obj_t* downLine;
    static short upEndpointsX = 140;
    static short upEndpointsY = 100;
    static short downEndpointsY = upEndpointsY + 30;
    static lv_point_t upEndpoints[3] = {
        {static_cast<short>(-15_s + upEndpointsX), static_cast<short>(upEndpointsY)},
        {static_cast<short>(upEndpointsX), static_cast<short>(-15_s + upEndpointsY)},
        {static_cast<short>(15_s + upEndpointsX), static_cast<short>(upEndpointsY)}};
    static lv_point_t downEndpoints[3] = {
        {static_cast<short>(-15_s + upEndpointsX), static_cast<short>(downEndpointsY)},
        {static_cast<short>(upEndpointsX), static_cast<short>(15_s + downEndpointsY)},
        {static_cast<short>(15_s + upEndpointsX), static_cast<short>(downEndpointsY)}};
    static lv_obj_t* readyLabel;
    static const char* readyYes = "#ffffff Flywheel";
    static const char* readyNo = "#646464 Flywheel";
    static lv_obj_t* intakingLabel;
    static const char* intakingYes = "#ffffff Intaking";
    static const char* intakingNo = "#646464 Intaking";
    static lv_obj_t* clearingLabel;
    static const char* clearingYes = "#ffffff Clearing";
    static const char* clearingNo = "#646464 Clearing";
    static lv_color_t inactiveLine = lv_color_make(100, 100, 100);
    static lv_color_t activeLine = lv_color_make(255, 255, 255);
    static bool init = false;
    static vector<lv_point_t> linePositions = {
        {-50, 0},
        {0, 0},
        {-95, -38},
        {-64, -5},
        {-97, -103},
        {-97, -51},
        {-60, -152},
        {-93, -115},
        {-50, -158},
        {0, -158}};
    static vector<vector<lv_point_t>> lineEndpoints = {};
    if (!init)
    {
        for (auto& p : linePositions)
        {
            p.x += 140;
            p.y += 200;
        }
        for (int i = 0; i < linePositions.size() / 2; i++)
        {
            vector<lv_point_t> arr = {};
            arr.push_back(linePositions[2 * i]);
            arr.push_back(linePositions[2 * i + 1]);
            lineEndpoints.push_back(arr);
        }
        init = true;
    }
    if (refresh)
    {
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(50, 50, 50), 0);
        lines.clear();
        for (int i = 0; i < lineEndpoints.size(); i++)
        {
            bool active = (diskMask >> i) & 1;
            lv_obj_t* line = lv_line_create(lv_scr_act());
            // lv_obj_align(line, LV_ALIGN_CENTER, 0, 50);
            lv_line_set_points(line, lineEndpoints[i].data(), lineEndpoints[i].size());
            lv_obj_set_style_line_width(line, 5, 0);
            lv_obj_set_style_line_rounded(line, true, 0);
            if (active)
            {
                lv_obj_set_style_line_color(line, activeLine, 0);
            }
            else
            {
                lv_obj_set_style_line_color(line, inactiveLine, 0);
            }
            lines.push_back(line);
        }
        upLine = lv_line_create(lv_scr_act());
        lv_line_set_points(upLine, upEndpoints, 3);
        lv_obj_set_style_line_width(upLine, 5, 0);
        lv_obj_set_style_line_rounded(upLine, true, 0);
        if (direction == 1)
        {
            lv_obj_set_style_line_color(upLine, activeLine, 0);
        }
        else
        {
            lv_obj_set_style_line_color(upLine, inactiveLine, 0);
        }
        downLine = lv_line_create(lv_scr_act());
        lv_line_set_points(downLine, downEndpoints, 3);
        lv_obj_set_style_line_width(downLine, 5, 0);
        lv_obj_set_style_line_rounded(downLine, true, 0);
        if (direction == -1)
        {
            lv_obj_set_style_line_color(downLine, activeLine, 0);
        }
        else
        {
            lv_obj_set_style_line_color(downLine, inactiveLine, 0);
        }
        readyLabel = lv_label_create(lv_scr_act());
        lv_label_set_recolor(readyLabel, true);
        if (flywheelReady)
        {
            lv_label_set_text(readyLabel, readyYes);
        }
        else
        {
            lv_label_set_text(readyLabel, readyNo);
        }
        lv_obj_align(readyLabel, LV_ALIGN_CENTER, 0, -50);

        clearingLabel = lv_label_create(lv_scr_act());
        lv_label_set_recolor(clearingLabel, true);
        if (clearingDisks)
        {
            lv_label_set_text(clearingLabel, clearingYes);
        }
        else
        {
            lv_label_set_text(clearingLabel, clearingNo);
        }
        lv_obj_align(clearingLabel, LV_ALIGN_CENTER, 0, -30);

        intakingLabel = lv_label_create(lv_scr_act());
        lv_label_set_recolor(intakingLabel, true);
        if (intaking)
        {
            lv_label_set_text(intakingLabel, intakingYes);
        }
        else
        {
            lv_label_set_text(intakingLabel, intakingNo);
        }
        lv_obj_align(intakingLabel, LV_ALIGN_CENTER, 0, -10);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        bool active = (diskMask >> i) & 1;
        if (active)
        {
            lv_obj_set_style_line_color(lines[i], activeLine, 0);
        }
        else
        {
            lv_obj_set_style_line_color(lines[i], inactiveLine, 0);
        }
    }
    lv_obj_set_style_line_color(upLine, inactiveLine, 0);
    lv_obj_set_style_line_color(downLine, inactiveLine, 0);
    if (direction == 1)
    {
        lv_obj_set_style_line_color(upLine, activeLine, 0);
    }
    else if (direction == -1)
    {
        lv_obj_set_style_line_color(downLine, activeLine, 0);
    }
    if (flywheelReady)
    {
        lv_label_set_text(readyLabel, readyYes);
    }
    else
    {
        lv_label_set_text(readyLabel, readyNo);
    }
    if (clearingDisks)
    {
        lv_label_set_text(clearingLabel, clearingYes);
    }
    else
    {
        lv_label_set_text(clearingLabel, clearingNo);
    }
    if (intaking)
    {
        lv_label_set_text(intakingLabel, intakingYes);
    }
    else
    {
        lv_label_set_text(intakingLabel, intakingNo);
    }
}
#elif BOT == 2
AutoIntake::AutoIntake(LineCounter& counter, MotorGroup& sling, MotorGroup& intake, pneumatics& release, std::function<bool()> atBack, bool pneumaticReleaseState)
  : counter(counter), slingMot(sling), intakeMot(intake), release(release), atBack(atBack), pneumaticsReleaseState(pneumaticReleaseState)
{
}
void AutoIntake::disable()
{
    enabled = false;
}
void AutoIntake::enable()
{
    enabled = true;
}
void AutoIntake::updateValues()
{
    if (enabled)
    {
        int countThrough = counter.getCountIn();
        if (atBack() && timeSinceRelease > 1000)
        {
            isAtBack = true;
        }
        if (!isAtBack && timeSinceRelease > 1000)
        {
            slingMot.spinVolt(reverse, 100);
        }
        else if (atBack() && timeSinceRelease > 500)
        {
            release.set(!pneumaticsReleaseState);
            slingMot[0].spinToPosition(0, degrees, 100, velocityUnits::pct, false);
        }
        if (lastCount == 2 && counter.getCountOut() == 3)
        {
            continueTime = 300;
            continueDirection = 1;
        }
        if (countThrough > 3 && lastCount != counter.getCountOut())
        {
            continueTime = 500;
            continueDirection = -1;
        }
        if (targetCount == counter.getCountOut() && lastCount != counter.getCountOut())
        {
            continueDirection = 1;
            continueTime = 500;
        }
        // I'm fairly sure that as of writing this,
        // there is no way to accidentally get more than 3 disks through the intake (bc of protection in intake function), but might as well have protection for it
        if ((countThrough > 3 && counter.pressing()) || reversed || (continueDirection < 0 && continueTime > 0))
        {
            intakeMot.spinVolt(vex::reverse, 100);
        }
        else if ((targetCount > counter.getCountOut() || (continueDirection > 0 && continueTime > 0) || forward) && countThrough <= 3 && isAtBack)
        {
            intakeMot.spin(fwd, 65);
        }
        else
        {
            intakeMot.stop();
        }
        lastCount = counter.getCountOut();
    }
    continueTime -= 10;
    timeSinceRelease += 10;
}
void AutoIntake::decreaseCount()
{
    counter.setCount(2);
}
void AutoIntake::setFiring()
{
    if (release.value() != pneumaticsReleaseState)
    {
        cout << "fire" << endl;
        targetCount = 0;
        counter.reset();
        timeSinceRelease = 0;
        release.set(pneumaticsReleaseState);
        isAtBack = false;
    }
}
void AutoIntake::intake()
{
    intakeMultiple(1);
}
void AutoIntake::intakeMultiple(int count)
{
    targetCount = counter.getCountOut() + count;
    if (targetCount > 3)
    {
        targetCount = 3;
    }
}
void AutoIntake::reverseMotor()
{
    reversed = true;
}
void AutoIntake::moveForward()
{
    forward = true;
}
void AutoIntake::stopDriverSpins()
{
    reversed = false;
    forward = false;
}
void AutoIntake::setCount(int count)
{
    counter.setCount(count);
}
#endif
