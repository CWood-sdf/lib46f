#include "Controllers.h"
double sign(double v)
{
    if (v == 0.0)
    {
        return 0;
    }
    return v / abs(v);
}

// void BasicWheelController::popTopTurnPid(){
//   auto& a = customPidsTurn.getEnd();
//   turnCtrl -= a;
//   customPidsTurn.popEnd();
// }
// void BasicWheelController::addTurnPid(PidAdder a){
//   turnCtrl += a;
//   customPidsTurn.push(a);

// }
// void BasicWheelController::addTurnPid(double p, double i, double d){
//   addTurnPid(PidAdder(p, i, d));
// }
double WheelController::botAngle()
{
    return chassis->botAngle();
}
PVector WheelController::botPos()
{
    return chassis->botPos();
}
// Add a function to be called at a specified distance
void WheelController::addDistFn(double dist, std::function<void()> fn)
{
    distFns[dist] = fn;
}
// Reuse the old map
void WheelController::reuseDistFns()
{
    distFns = oldFns;
}
void WheelController::setFn(std::function<void()> fn)
{
    afterTurn = fn;
    hasFn = true;
}
void WheelController::callFn()
{
    if (hasFn)
    {
        afterTurn();
    }
    hasFn = false;
}
void WheelController::reuseFn()
{
    hasFn = true;
}

// A hard brake

void WheelController::setOldDistFns()
{
    oldFns = distFns;
}
void WheelController::useDistFns(double dist)
{
    for (auto it = distFns.begin(); it != distFns.end() /* not hoisted */; /* no increment */)
    {
        if ((*it).first > dist)
        {

            (*it).second();
            distFns.erase(it++); // or "it = m.erase(it)" since C++11
        }
        else
        {
            ++it;
        }
    }
}
void WheelController::turnTo(std::function<double()> angleCalc)
{
    //
    auto oldAngleCalc = angleCalc;
    bool add180 = isBlue() && !callingInDrive;
    // If the auton is on the other side, turn to the opposite angle
    double angle = posNeg180(angleCalc() + add180 * 180);
    // cout << "A: " << angle << endl;
    // cout << "A2: " << botAngle() << endl;
    // if(!callingInDrive && reversed){
    //   angle += 90;
    // }
    int timeIn = 0;
    [[maybe_unused]] int i = 0;
    // Get the normAngle
    double normAngle = posNeg180(angle - posNeg180(botAngle()));
    // init PID
    turnCtrl.setTarget(0);
    int sleepTime = 20;
    int minTimeIn = 200;
    double degRange = 4.0;
    int speedLimit = chassis->getSpeedLimit();
    int timeSpent = 0;

    //
    //
    //
    // cout << normAngle << endl;

    // It is off by 4 degrees, because:
    //     A: if it is within 4 degrees, without the minSpeed
    //           it is too slow; but, with minSpeed, it overshoots too much
    while (timeIn * sleepTime < minTimeIn)
    {

        double speed = turnCtrl.getVal(normAngle);
        if (abs(speed) < 30)
        {
            speed = 30 * sign(speed);
        }
        chassis->turnLeft(speed > speedLimit ? speedLimit : speed);
        s(sleepTime);

        angle = posNeg180(angleCalc() + add180 * 180);

        normAngle = posNeg180(angle - botAngle());
        if (abs(normAngle) < degRange)
        {
            timeIn++;
        }
        else
        {
            timeIn = 0;
        }
        timeSpent += sleepTime;
        if (timeSpent > 5000)
        {
            cout << "Turn fail (timeout)" << endl;
            break;
        }
        //
    }
    chassis->holdBrake();

    // s(300);

    cout << "current: " << botAngle() << endl;
    cout << "target: " << angleCalc() << endl;
}
void WheelController::turnTo(double angle)
{
    turnTo([=]()
        { return angle; });
}
// Implement faceTarget
void WheelController::faceTarget(PVector target)
{
    if (!isRed())
    {
        target.x *= -1.0;
        target.y *= -1.0;
    }
    callingInDrive = true;
    turnTo([=]()
        { return botPos().angleTo(target); });
    // Reinvoke just in case the turn messed things up
    // turnTo(botPos().angleTo(target));
    callingInDrive = false;
}

bool WheelController::isMoving()
{
    return moving;
}
#ifndef WINDOWS
WheelController::chain_method WheelController::estimateStartPos(PVector v, double a)
{
    cout << sign(botPos().x) << ", " << sign(v.x) << endl;
    // If it thinks it is at (0, 0), just use the given value

    if (isBlue())
    {
        chassis->pos.setPos(autonReversePosition(v), autonReverseAngle(a));
    }
    else
    {
        chassis->pos.setPos(v, a);
    }
    CHAIN;
}
#endif
WheelController::chain_method WheelController::forceEarlyExit()
{
    exitEarly = true;
    CHAIN;
}

WheelController::chain_method WheelController::setExitMode(exitMode m)
{
    BrakeMode = m;
    CHAIN;
}

WheelController::chain_method WheelController::setExitDist(double v)
{
    exitDist = v;
    CHAIN;
}
PVector WheelController::getLastTarget()
{
    return lastTarget;
}
WheelController::chain_method WheelController::prevStopExit()
{
    stopExitPrev = true;
    CHAIN
}
WheelController::chain_method WheelController::setPathRadius(double r)
{
    pathRadius = r;
    CHAIN
}
double WheelController::getPathRadius()
{
    return pathRadius;
}
WheelController::chain_method WheelController::setFollowPathDist(double d)
{
    followPathDist = d;
    CHAIN;
}
double WheelController::getFollowPathDist()
{
    return followPathDist;
}
WheelController::chain_method WheelController::setFollowPathMaxTimeIn(int t)
{
    followPathMaxTimeIn = t;
    CHAIN;
}
int WheelController::getFollowPathMaxTimeIn()
{
    return followPathMaxTimeIn;
}
template <class Arr>
size_t WheelController::getNearest(Arr arr, PVector obj)
{
    size_t i = 0;
    double minDist = obj.dist2D(arr[0]);
    for (int j = 1; j < arr.size(); j++)
    {
        if (obj.dist2D(arr[j]) < minDist)
        {
            i = j;
        }
    }
    return i;
}
template <class Arr>
size_t WheelController::getNearest(Arr arr, PVector obj, size_t start)
{
    size_t i = start;
    double minDist = obj.dist2D(arr[start]);
    if (start == arr.size() - 1)
    {
        return start;
    }
    for (int j = start; j < arr.size(); j++)
    {
        if (obj.dist2D(arr[j]) < minDist)
        {
            i = j;
        }
    }
    return i;
}

void WheelController::ramseteFollow(VectorArr arr, bool isNeg)
{
    generalFollow(arr, defaultRamsete, isNeg);
}

// }

void WheelController::driveTo(double x, double y)
{
    generalFollow({PVector(x, y)}, defaultPid, false);
    // faceTarget({x, y});
    // driveDistance(botPos().dist2D({x, y}), defaultPid);
    // cout << "Sorta target: " << PVector(x, y) << ", " << botPos() << endl;
}
void WheelController::backInto(double x, double y)
{
    generalFollow({PVector(x, y)}, defaultPid, true);
    // turnTo(botPos().angleTo({x, y}) + 180);
    // backwardsDriveDistance(botPos().dist2D({x, y}), defaultPid);
    // cout << "Sorta target: " << PVector(x, y) << ", " << botPos() << endl;
}

/**
 * Boolean Settings:
 *  - Turn at the start
 *  - Use forward distance
 * Other settings:
 *  - virtual pursuit distance
 *  - exit distance
 *  - exit mode
 *  - path radius
 *  - follow path distance
 *  - max time in
 */
void WheelController::generalFollowTurnAtStart(VectorArr& arr, double& purePursuitDist, bool& isNeg)
{
    this->drawArr = true;
    auto arrCopy = arr;
    arrCopy.pushBase(botPos());
    // Construct the original bezier
    VectorArr bezier = bezierCurve(arrCopy);

    // cout << "sdfa" << endl; s(100);
    this->publicPath = bezier;
    // Turn to the first point purePursuitDist away
    //  get first point
    double dist = botPos().dist2D(bezier[0]);
    int i = 0;
    // cout << "Cool" << endl;
    while (dist < purePursuitDist)
    {
        dist += bezier[i].dist2D(bezier[i + 1]);
        i++;
    }
    callingInDrive = true;
    auto angle = botPos().angleTo(bezier[i - 1]);
    turnTo(angle + 180.0 * isNeg);
    callingInDrive = false;
    // cout << "Cool" << endl;
    cout << "Turn Done" << endl;
    afterTurn();
    afterTurn = []() {};
}
PVector WheelController::generalFollowGetVirtualPursuit(PVector& pursuit, Controller* controller)
{
    PVector virtualPursuit = pursuit;
    if (!(botPos().dist2D(pursuit) < controller->settings.virtualPursuitDist && pursuit == path.last()))
    {
        return virtualPursuit;
    }
    // A vector that is parallel wih last point
    PVector last = (PVector)path.last() - path[path.size() - 4];
    // Distance to be added so that virtualPursuit is still purePursuitDist away from bot
    double addDist = controller->settings.virtualPursuitDist - botPos().dist2D(pursuit);
    // Make last to be proper size

    last *= addDist / last.mag();
    virtualPursuit += last;
    return virtualPursuit;
}
double WheelController::generalFollowGetDist(int& bezierIndex, Controller* controller, PVector& pursuit)
{
    double dist = 0.0;
    if (controller->settings.useDistToGoal)
    {
        dist = botPos().dist2D(pursuit);
        // Make the dist signed so that PID will work
        // If we're not at the end, just return the distance, it won't be signed bc the point is always ahead
        if (bezierIndex != path.size() - 1)
        {
            return dist;
        }
        // Get a point near the end
        PVector pathNearEnd = path[path.size() - 3];
        // Multiply dist by -1 if the bot is past the target
        // Something abt dot products is that they are negative when the angle between is greater than 90
        double distSign = sign((pathNearEnd - path.last()).dot(botPos() - path.last()));
        dist *= distSign;
        return dist;
    }
    // If we're not near the end, let's not do fancy math
    if (bezierIndex != path.size() - 1)
    {
        return botPos().dist2D(path[bezierIndex]);
    }
    // A vector that is parallel with the end
    PVector pathDir = (PVector)path[bezierIndex] - path[bezierIndex - 1];
    // The bot's point centered with the 2nd to last point
    PVector botDir = botPos() - path[bezierIndex - 1];
    // The angle to make the parallel vector straight
    double pathDirAngle = pathDir.heading();
    // Rotate the vectors back by the pathDirAngle
    pathDir.rotateXY(-pathDirAngle);
    botDir.rotateXY(-pathDirAngle);
    // The dist is the difference in the y's
    dist = pathDir.y - botDir.y;
    return dist;
}
void WheelController::generalFollow(VectorArr& arr, Controller* controller, bool isNeg)
{
    // double purePursuitDist = controller->settings.followPathDist; // Distance to pure pursuit target

    controller->init();
    cout << "blue: " << isBlue() << endl;
    if (isBlue())
    {
        for (auto& a : arr)
        {
            a = autonReversePosition(a);
        }
    }

    if (controller->settings.turnAtStart)
    // Simple initialization and turn to first point
    {
        generalFollowTurnAtStart(arr, controller->settings.followPathDist, isNeg);
    }

    path.make(arr, chassis);

    auto bezier = path.getBezier();

    // The index of the pursuit target
    int bezierIndex = 0;

    // The pursuit target
    PVector pursuit = path[bezierIndex];

    // Allow the array to be drawn
    this->drawArr = true;
    this->publicPath = bezier;

    // A timer
    timer t = timer();
    int timeIn = 0;     // The amount of time spent near the target
    int sleepTime = 10; // The sleep time

    double minAllowedDist = controller->settings.exitDist; // The maximum distance from target before starting timeIn count

    const bool debug = Greg.ButtonA.pressing();
#ifdef DEBUG
    struct
    {
        timer t;
        vector<double> outSpeeds, encSpeeds, targSpeeds, angles, cp, cd, sp, sd;
        vector<PVector> pos, pursuit;
        void add(double out, double enc, double targ, PVector p, double angle, double acp, double acd, double asp, double asd, PVector apursuit)
        {
            outSpeeds.push_back(out);
            encSpeeds.push_back(enc);
            targSpeeds.push_back(targ);
            pos.push_back(p);
            angles.push_back(angle);
            cp.push_back(acp);
            cd.push_back(acd);
            sp.push_back(asp);
            sd.push_back(asd);
            pursuit.push_back(apursuit);
        }
        void flush()
        {
            for (int i = 0; i < outSpeeds.size(); i++)
            {
                cout << "%"
                     << "o: " << outSpeeds[i] << ", "
                     << "e: " << encSpeeds[i] << ", "
                     << "t: " << targSpeeds[i] << ", "
                     << "mp: " << pos[i].x << "@" << pos[i].y << ", "
                     << "ma: " << angles[i] << ", "
                     << "sp: " << sp[i] << ", "
                     << "sd: " << sd[i] << ", "
                     << "cp: " << cp[i] << ", "
                     << "cd: " << cd[i] << ", "
                     << "mpu: " << pursuit[i].x << "@" << pursuit[i].y << endl;
                s(20);
            }
            cout << std::flush;
            outSpeeds.clear();
            encSpeeds.clear();
            targSpeeds.clear();
            pos.clear();
            angles.clear();
            cp.clear();
            cd.clear();
            sp.clear();
            sd.clear();
            pursuit.clear();
        }
    } realTime;
#endif
    // Save the current distance fns
    setOldDistFns();

    // The time that the robot has spent not moving
    int timesStopped = 0;

    // Set a flag
    moving = true;

    // The index of the last path point that the robot was on
    int lastIndex = 0;

    // The distance from the target
    double dist = 0.0;
#ifdef DEBUG
    if (debug)
    {
        cout.precision(3);
        cout << "%"
             << "frameRate: 40, "
             << "main.pos: def mp, "
             << "main.angle: def ma, "
             << "main.pursuit: def mpu, "
             << "outputVel: def o, encVel: def e, targetVel: def t, slaveD: def sd, slaveP: def sp, ctrlP: def cp, ctrlD: def cd" << endl;
        // Loop through path and print out all the points
        for (int i = 0; i < path.size(); i++)
        {
            cout << "%main: " << path[i].bezierPt.x << "@" << path[i].bezierPt.y << "\n";
            s(15);
        }
        cout << flush;
    }
#endif
    cout << "Target: " << path.last().bezierPt << endl;
    // Loop
    while (timeIn * sleepTime < controller->settings.maxTimeIn)
    {

        // Get the nearest pure pursuit position
        int nearestIndex = getNearest(path, botPos(), lastIndex);
        lastIndex = nearestIndex;
        if (exitEarly)
        {
            cout << "Exit due to external thread request" << endl;
            exitEarly = false;
            break;
        }

        // Keep the Pure Pursuit target purePursuitDist inches away from the bot
        while (pursuit.dist2D(botPos()) < controller->settings.followPathDist && pursuit != path.last())
        {
            pursuit = path[bezierIndex];
            ++bezierIndex;
        }

        if (bezierIndex >= path.size())
        {
            bezierIndex = path.size() - 1;
        }
        // Near the target, increment timeIn
        if (dist < minAllowedDist && pursuit == path.last())
        {
            timeIn++;
        }
        else
        {
            timeIn = 0;
        }

        // The distance to the pursuit target
        dist = botPos().dist2D(pursuit);
        // If the bot's not moving, and it's not currently accelerating
        if (chassis->pos.velocity() < 0.05 && t.time(timeUnits::msec) > 1000)
        {
            timesStopped++;
        }
        else
        {
            timesStopped = 0;
        }
        // 50 ms not moving -> exit
        if (timesStopped * sleepTime > 50 && !stopExitPrev)
        {
            cout << "Stop Exit" << endl;
            break;
        }
        // Use the distFns for the current dist
        useDistFns(botPos().dist2D(path.last()));
        // The point extended beyond the path to make sure normAngle doesn't get big near the target
        PVector virtualPursuit = generalFollowGetVirtualPursuit(pursuit, controller);

        dist = generalFollowGetDist(bezierIndex, controller, pursuit);

        // Angle of robot to target
        double angle = baseAngle(botPos().angleTo(virtualPursuit));

        // The angle that it needs to travel at
        double normAngle = posNeg180(angle - botAngle() + 180 * isNeg);

        // Input to speed controller
        Controller::Input input = Controller::Input();
        input.angleTarget = angle;
        input.currentAngle = posNeg180(botAngle() + 180 * isNeg);
        input.target = virtualPursuit;
        input.position = botPos();
        input.dist = dist;
        auto copy = path[bezierIndex];
        copy.bezierPt = virtualPursuit;
        input.targetPt = copy;
        input.chassis = chassis;

        auto speeds = controller->followTo(input);
        double speed = 0;
        switch (speeds.first.second)
        {
        case Controller::ForwardVel::inps:
            speed = chassis->realToPct(speeds.first.first);
            break;
        case Controller::ForwardVel::pct:
            speed = speeds.first.first;
            break;
        }
        // Slew speed
        if (abs(speed) > abs(path[nearestIndex].targetSpeed))
        {
            speed = path[nearestIndex].targetSpeed * sign(speed);
        }
        if (abs(speed) > abs(chassis->getSpeedLimit()))
        {
            speed = chassis->getSpeedLimit() * sign(speed);
        }
        // The extra speed to add to the right side
        double rightExtra = 0;
        {
            double targetRobotVel = chassis->pctToReal(speed);
            switch (speeds.second.second)
            {
            case Controller::AngularVel::curvature:
                rightExtra = chassis->realToPct(speeds.second.first * (chassis->getTrackWidth() + 3.0) * targetRobotVel / 2.0);
                while (abs(rightExtra) + abs(speed) > 100.0)
                {
                    speed = (abs(speed) - 1.0) * sign(speed);
                    targetRobotVel = chassis->pctToReal(speed);
                    rightExtra = chassis->realToPct(speeds.second.first * (chassis->getTrackWidth() + 3.0) * targetRobotVel / 2.0);
                    if (abs(speed) < 1.0)
                        break;
                }

                break;
            case Controller::AngularVel::pctDiff:
                rightExtra = speeds.second.first;
                break;
            case Controller::AngularVel::radps:
                rightExtra = chassis->realToPct(speeds.second.first * chassis->getTrackWidth() / -2.0);
                while (abs(rightExtra) + abs(speed) > 100.0)
                {
                    speed = (abs(speed) - 1.0) * sign(speed);
                    if (abs(speed) < 1.0)
                        break;
                }
                break;
            }
        }
        if (isNeg)
        {
            // rightExtra *= -1;
            speed *= -1.0;
        }
        // Mindblowing lines right here
        // Move the robot
        chassis->driveFromDiff(speed, -rightExtra);
        // Sleep (WOW, HE'S A GENIUS)

        s(sleepTime);

#ifdef DEBUG
        if (debug)
        {
            realTime.add(speed, chassis->pos.velocity(), path[nearestIndex].targetSpeed, botPos(), botAngle(), -rightExtra, 0, normAngle, dist, pursuit);
        }
#endif
    }
    moving = false;
    // Stop drawing the path
    // De-init code
    {
        // Set the last target for external stuff
        lastTarget = bezier.last();
        // Stop the bot
        switch (BrakeMode)
        {
        case exitMode::normal:
            chassis->holdBrake();
            break;
        case exitMode::coast:
            chassis->coastBrake();
            break;
        case exitMode::nothing:
            break;
        }
        this->drawArr = false;
        cout << "Path stop" << endl;
        // Print postion and target position
        cout << botPos() << ", " << bezier.last() << endl;
        exitDist = 0.0;
    }
    stopExitPrev = false;
    controller->deInit();
// Print all the lists
#ifdef DEBUG
    if (debug)
    {
        realTime.flush();
        cout << "%outputVel: fit, encVel: fit, targetVel: fit, slaveP: fit, slaveD: fit, ctrlP: fit, ctrlD: fit" << endl;
        s(20000);
    }
#endif
}
void WheelController::generalDriveDistance(double targetDist, bool isNeg, BasicPidController* pid)
{
    PVector startPos = chassis->botPos();
    double startAngle = chassis->botAngle();
    int timeIn = 0;
    int maxTimeIn = pid->settings.maxTimeIn;
    double maxDist = pid->settings.followPathDist;
    int sleepTime = 10;
    setOldDistFns();
    moving = true;
    double dist = 1000.0;
    timer t = timer();
    int timesStopped = 0;
    pid->init();
    double lastSpeed = 0.0;

    // cout << "%ctrlP: def d" << endl;
    while (1)
    {
        // Basic exit conditions
        if (timeIn * sleepTime > maxTimeIn)
        {
            break;
        }
        // 50 ms not moving -> exit
        if (timesStopped * sleepTime > 50 && !stopExitPrev)
        {
            cout << "Stop Exit" << endl;
            break;
        }
        // If the bot's not moving, and it's not currently accelerating
        if (chassis->pos.velocity() < 0.1 && t.time(timeUnits::msec) > 1000)
        {
            timesStopped++;
        }
        else
        {
            timesStopped = 0;
        }
        if (exitEarly)
        {
            cout << "Exit due to external thread request" << endl;
            exitEarly = false;
            break;
        }
        double angle = chassis->botAngle();

        PVector pos = chassis->botPos();
        // auto newRotation = chassis->rightWheels[0].rotation(rev);
        dist = targetDist - pos.dist2D(startPos);
        useDistFns(dist);
        // If near target, start the timer
        if (abs(dist) < pid->settings.exitDist)
        {
            timeIn++;
        }
        else
        {
            timeIn = 0;
        }
        Controller::Input input = Controller::Input();
        // Construct the inpur
        input.angleTarget = startAngle;
        input.currentAngle = angle;
        input.chassis = chassis;
        input.dist = dist > maxDist ? maxDist : dist;
        auto speeds = pid->followTo(input);
        double speed = 0;
        // Convert the speed to a percentage
        switch (speeds.first.second)
        {
        case Controller::ForwardVel::inps:
            speed = chassis->realToPct(speeds.first.first);
            break;
        case Controller::ForwardVel::pct:
            speed = speeds.first.first;
            break;
        }

        double rightExtra;
        // get the turn speed
        {
            double targetRobotVel = chassis->pctToReal(speed);
            switch (speeds.second.second)
            {
            case Controller::AngularVel::curvature:
                rightExtra = chassis->realToPct(speeds.second.first * (chassis->getTrackWidth() + 3.0) * targetRobotVel / 2.0);
                break;
            case Controller::AngularVel::pctDiff:
                rightExtra = speeds.second.first;
                break;
            case Controller::AngularVel::radps:
                rightExtra = chassis->realToPct(speeds.second.first * chassis->getTrackWidth() / -2.0);
                break;
            }
        }
        if (abs(speed) > chassis->getSpeedLimit())
        {
            speed = chassis->getSpeedLimit() * sign(speed);
        }
        if (isNeg)
        {
            speed *= -1.0;
        }
        // double speedSign = sign(speed);
        // Slew speed on vf2 = vi2 + 2ad
        // double maxAccel = chassis->maxAcc;
        // double maxDecel = chassis->maxDAcc;
        // if (speed > lastSpeed)
        // {
        //   double maxSpeed = sqrt(pow(chassis->pctToReal(lastSpeed), 2) - 2 * maxDecel * sleepTime);
        //   if (abs(speed) < maxSpeed)
        //   {
        //     speed = maxSpeed * speedSign;
        //   }
        // }
        // else if (speed < lastSpeed)
        // {
        //   double maxSpeed = sqrt(pow(chassis->pctToReal(lastSpeed), 2) + 2 * maxAccel * sleepTime);
        //   if (abs(speed) > maxSpeed)
        //   {
        //     speed = maxSpeed * speedSign;
        //   }
        // }
        chassis->driveFromDiff(speed, -rightExtra);
        lastSpeed = speed;
        s(sleepTime);
    }
    moving = false;
    // Stop drawing the path
    // De-init code
    {
        // Stop the bot
        switch (BrakeMode)
        {
        case exitMode::normal:
            chassis->holdBrake();
            break;
        case exitMode::coast:
            chassis->coastBrake();
            break;
        case exitMode::nothing:
            break;
        }
        this->drawArr = false;
        cout << "PID stop: ";
        // Print postion and target position
        cout << -dist + targetDist << ", " << targetDist << ", " << botPos() << endl;
        cout << botAngle() - startAngle << endl;
        exitDist = 0.0;
    }
    stopExitPrev = false;
    pid->deInit();
}
void WheelController::driveDistance(double dist)
{
    generalDriveDistance(dist, false, defaultPid);
}
void WheelController::backwardsDriveDistance(double dist)
{
    generalDriveDistance(dist, true, defaultPid);
}
// The beefiest function in this file
void WheelController::purePursuitFollow(VectorArr arr, bool isNeg)
{
    generalFollow(arr, defaultPurePursuit, isNeg);
}
bool WheelController::isRed()
{
    return !reversed;
}
void WheelController::setRed()
{
    reversed = false;
}
void WheelController::setBlue()
{
    reversed = true;
}
bool WheelController::isBlue()
{
    return reversed;
}

void MechWheelController::moveAt(double angle, double speed, double turnSpeed)
{
    // There are actual equations governing this
    cout << "MechWheelController::moveAt is not implemented" << endl;
}
void MechWheelController::followPath(VectorArr arr, double targetAngle)
{
}
void MechWheelController::driveTo(PVector pos, double finalAngle)
{
    followPath({pos}, finalAngle);
}
void MechWheelController::driveTo(double x, double y, double finalAngle)
{
    driveTo(PVector(x, y), finalAngle);
}
void MechWheelController::backInto(double x, double y, double finalAngle)
{
    backInto(PVector(x, y), finalAngle);
}
void MechWheelController::backInto(PVector pos, double finalAngle)
{
    backwardsFollow({pos}, finalAngle);
}
void MechWheelController::backwardsFollow(VectorArr arr, double targetAngle)
{
}
