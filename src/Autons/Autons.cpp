#include "src/Autons/Autons.h"
#if BOT == 1
void waitIntakeDone()
{
    while (intakeController.clearingDisks || intakeController.intaking)
    {
        wait(20, msec);
    }
}
void waitFlywheelReady(int minTime = 0)
{
    int readyCount = 5;
    bool canExit = false;
    while (readyCount > 0 || minTime > 0)
    {
        if (flyTBH.ready())
        {
            readyCount--;
        }
        else
        {
            readyCount = 8;
        }
        if (readyCount < 0)
        {
            canExit = true;
        }
        if (canExit && minTime < 0)
        {
            break;
        }
        wait(50, msec);
        minTime -= 50;
    }
}
void waitFlywheelNotReady(int minTime = 0)
{
    int readyCount = 5;
    bool canExit = false;
    while (readyCount > 0 || minTime > 0)
    {
        if (!flyTBH.ready())
        {
            readyCount--;
        }
        else
        {
            readyCount = 5;
        }
        if (readyCount < 0)
        {
            canExit = true;
        }
        if (canExit && minTime < 0)
        {
            break;
        }
        wait(50, msec);
        minTime -= 50;
    }
}
void graphFlywheelTBH(bool);
Auton leftA = "Left" + []()
{
    // flyTBH.setDisabled(true);
    // bos::bosFns.moveCurrentLeft();
    // bos::bosFns.moveCurrentLeft();
    LinkedList<bool> flywheelReady = {};
    // bos::bosFns.moveCurrentLeft();
    // bos::bosFns.moveCurrentLeft();
    // bos::bosFns.getCurrent()->call(true);
    intakeController.disable();
    flyTBH.setTargetSpeed(528);
    flyTBH.setDisabled(true);
    s(100);
    flywheelNm.spinVolt(fwd, 528.0 / 6.0);
    // flywheelNm.spin(fwd, 100);
    wc.estimateStartPos(PVector(-61.39, 41.17), 88.59);

    // wc.faceTarget({49.60, 49.88});
    // wc.faceTarget({49.60, 49.88});
    // intake.spin(fwd, 100);
    // s(2000);
    // flyTBH.setDisabled(false);
    s(100);
    // Fire
    // waitFlywheelReady();
    s(3500);
    cout << "0" << endl;
    intake.spin(fwd, 50);
    s(600);
    cout << "1" << endl;
    intake.stop(hold);
    // spinRoller();
    // chassis.driveFromDiff(20, 0);
    // s(300);
    // chassis.coastBrake();

    // wc.faceTarget({49.60, 49.88});
    // s(500);
    // waitFlywheelReady();
    cout << 1.5 << endl;
    s(3000);
    intake.spin(fwd, 50);
    // waitFlywheelNotReady(1000);
    s(1000);
    cout << "2" << endl;
    intake.stop(hold);
    // intakeController.setFiring();
    // waitIntakeDone();
    flyTBH.setTargetSpeed(0);
    wc.turnTo(90);
    spinRoller();
    flyTBH.setDisabled(false);
    // // // Drive to 3-stack
    // // goalRaise.open();
    // wc.driveTo(-30.48, 6.59);

    // // goalRaise.close();
    // // intakeController.intakeMultiple(3);
    // wc.faceTarget({49.42, 50.22});
    // // // Fire
    // intake.spin(fwd, 60);
    // intakeController.setFiring();
};
Auton rightA = "Right" + []()
{
    // wc.setBlue();
    cout << "Red: " << wc.isRed() << endl;
    intakeController.disable();
    wc.estimateStartPos(PVector(16.81, -62.47), 0);
    // pidController.settings.setBrakeMode(WheelController::exitMode::hold);
    // bos::bosFns.moveCurrentLeft();
    // bos::bosFns.moveCurrentLeft();
    // bos::bosFns.getCurrent()->call(true);
    flyTBH.setTargetSpeed(530);
    flyTBH.setDisabled(true);
    s(100);
    flywheelNm.spinVolt(fwd, 533.0 / 6.0);
    // flywheelNm.spin(fwd, 100);
    wc.estimateStartPos(PVector(-61.39, 41.17), 88.59);

    // wc.faceTarget({49.60, 49.88});
    // wc.faceTarget({49.60, 49.88});
    // intake.spin(fwd, 100);
    // s(2000);
    // flyTBH.setDisabled(false);
    s(100);
    // Fire
    // waitFlywheelReady();
    s(3500);
    cout << "0" << endl;
    intake.spin(fwd, 50);
    s(600);
    cout << "1" << endl;
    intake.stop(hold);

    flyTBH.setTargetSpeed(0);
    flyTBH.setDisabled(false);

    // // Fire
    // intakeController.setFiring();
    // // Move fwd to allow for roller
    wc.driveTo(21.59, -44.32);
    // // Drive to roller
    // wc.backwardsFollow(&pidController, {PVector(39.01, -56.1)});
    pidController.settings.setBrakeMode(WheelController::exitMode::hold);
    wc.backInto(37.18, -56.76);
    wc.turnTo(-10);
    chassis.driveFromDiff(-20, 0);
    s(300);
    chassis.coastBrake();
    spinRoller();
    // // Drive along 3-ground
    // wc.followPath(&pidController, {PVector(-14.26, -11.07)});
    // wc.faceTarget({48.57, 50.05});
    // // Fire
    // intakeController.setFiring();
};
bool intaking = false;
void launchDisks()
{
    intakeController.setFiring();
    intakeController.waitForFiring();
}
Auton skills = "Skills" + []()
{
    PVector goal = {46, 50};
    wc.setRed();
    flyTBH.setDisabled(false);
    flyTBH.setTargetSpeed(450);
    wc.estimateStartPos(PVector(-61.39, 41.34), 89.36);
    intakeController.enable();
    spinRoller();
    intakeController.enable();
    s(100);

    // Get first disk
    intakeController.intake();
    wc.turnTo(65.04);
    wc.driveDistance(21.29);
    wc.turnTo(90.79);
    launchDisks();
    flyTBH.setTargetSpeed(431);

    // Spin roller
    wc.turnTo(180.67);
    wc.backwardsDriveDistance(10.38);
    spinRoller();

    wc.driveDistance(2.23);

    // Next disk
    intakeController.intake();
    wc.turnTo(125.59);
    wc.driveDistance(37.53);
    wc.turnTo(75.17);
    launchDisks();
    flyTBH.setTargetSpeed(420);

    // Next disk
    intakeController.intake();
    wc.turnTo(127.38);
    wc.driveDistance(12.26);
    wc.turnTo(60.96);
    launchDisks();

    // Next disk
    intakeController.intake();
    flyTBH.setTargetSpeed(420);
    wc.turnTo(133.64);
    wc.driveDistance(19.87);
    wc.turnTo(44.00);
    launchDisks();

    // Big stack
    intakeController.intake();
    flyTBH.setTargetSpeed(450);
    wc.turnTo(125.84);
    wc.driveDistance(38.22);
    wc.turnTo(10.45);
    launchDisks();

    // Roller
    flyTBH.setTargetSpeed(535);
    wc.turnTo(143.77);
    wc.driveDistance(33.70);
    wc.turnTo(269.25);
    wc.backwardsDriveDistance(12);
    spinRoller();

    // Disk
    intakeController.intake();
    wc.driveDistance(9.08);
    wc.turnTo(250.34);
    wc.driveDistance(11.90);
    wc.turnTo(-89.06);
    launchDisks();

    // Last roller
    wc.turnTo(0);
    wc.backwardsDriveDistance(15.27);
    spinRoller();

    // Endgame
    wc.turnTo(-41.99);
    endgame.open();
};
Auton winPoint = "Win Point" + []()
{
    flyTBH.setTargetSpeed(485);
    intakeController.enable();
    wc.estimateStartPos(PVector(-60.36, 40.58), 91.46);
    spinRoller();
    s(20);
    if (intakeController.count == 0)
    {
        intakeController.intake();
    }
    wc.addDistFn(12, []()
        { chassis.setSpeedLimit(50); });
    wc.followPath(&pidController, {PVector(-48.31, 0.46), PVector(-12.31, -11.91)});
    chassis.setSpeedLimit(100);
    s(100);
    wc.faceTarget({49.93, 50.16});
    launchDisks();
    flyTBH.setTargetSpeed(487);
    intakeController.intake();
    wc.driveTo(-0.10, -23.97);
    s(100);
    wc.faceTarget({49.93, 50.16});
    launchDisks();
    intakeController.intake();
    wc.driveTo(40.68, -56.99);
    // wc.backInto(40.68, -61.12);
    wc.turnTo(0);
    wc.backwardsDriveDistance(12);
    spinRoller();
};
Auton skillsPt = "Skils driveTo" + []()
{
    flyTBH.setTargetSpeed(350);
    wc.estimateStartPos(PVector(-62.51, 40.91), 89.23);
    wc.driveTo(-42.04, 51.98);
    wc.faceTarget({49.93, 49.50});
    wc.backInto(-41.71, 62.05);
    // spinRoller();
    intakeController.intake();

    wc.followPath(&purePursuit, {PVector(-41.87, 45.54), PVector(-12.81, 36.29)});
    wc.faceTarget({49.93, 49.50});
    launchDisks();

    intakeController.intake();
    wc.driveTo(0.22, 23.74);
    wc.faceTarget({49.93, 49.50});
    launchDisks();

    intakeController.intake();
    wc.driveTo(12.94, 11.19);
    wc.faceTarget({49.93, 49.50});
    launchDisks();

    intakeController.intake();
    wc.driveTo(36.22, -12.25);
    wc.faceTarget({49.93, 49.50});
    launchDisks();

    wc.driveTo(57.36, -41.47);
    wc.turnTo(-89.85);
    wc.backwardsDriveDistance(4.01);
    spinRoller();

    wc.driveTo(41.50, -52.04);
    wc.faceTarget({-49.47, -50.06});
    wc.turnTo(0.16);
    wc.backwardsDriveDistance(9.30);
    spinRoller();

    wc.faceTarget({-72, 72});
};
#elif BOT == 2
void spinRoller()
{
    intakeController.disable();
    s(50);
    chassis.driveFromDiff(-40, 0);
    intake.spinVolt(fwd, 100);
    s(200);
    intake.stop(hold);
    intakeController.enable();
    chassis.driveFromDiff(40, 0);
    s(200);
}
void skillsSpinRoller()
{
    intakeController.disable();
    s(50);
    chassis.driveFromDiff(-40, 0);
    intake.spinVolt(fwd, 100);
    s(400);
    intake.stop(hold);
    intakeController.enable();
    chassis.driveFromDiff(40, 0);
    s(400);
}
void skillsFrontRoller()
{
    intakeController.disable();
    s(50);
    chassis.driveFromDiff(40, 0);
    intake.spinVolt(fwd, 100);
    s(200);
    intake.stop(hold);
    intakeController.enable();
    chassis.driveFromDiff(-40, 0);
    s(200);
}
void launchDisks()
{
    intakeController.setFiring();
}
void intakeCount(int count)
{
    intakeController.intakeMultiple(count);
}
Auton leftA = "Left" + []()
{
    // wc.setBlue();
    wc.estimateStartPos(PVector(-60.69, 40.42), 89.98);
    spinRoller();
    wc.path.setK(1.8);
    wc.followPath(&purePursuit, {PVector(-61.02, -10.76), PVector(-21.92, 4.74)});
    wc.faceTarget({49.10, 50.32});
    wc.driveDistance(5);
    wc.faceTarget({49.10, 50.32});
    s(800);
    launchDisks();
    // chassis.driveFromDiff(100, 0);
    // s(300);
    // launchDisks();
    // chassis.driveFromDiff(0, 0);
    // intakeCount(1);
    // s(3000);
    // wc.turnTo(154.05);
    // chassis.setSpeedLimit(40);
    // wc.driveDistance(17.70);
    // s(1000);
    // wc.faceTarget({49.76, 50.99});
    // wc.driveDistance(7);
    // wc.faceTarget({48, 48});
    // s(1000);
    // launchDisks();
};
Auton rightA = "Right" + []()
{
    wc.estimateStartPos(PVector(16.90, -55.18), 269.05);
    wc.backInto(37.54, -55.18);
    wc.turnTo(0.08);
    wc.backwardsDriveDistance(5.83);
    spinRoller();
    // intakeCount(2);
    wc.driveTo(-12.31, -12.58);
    wc.faceTarget({49.76, 52.99});
    s(500);
    launchDisks();
    // wc.backwardsDriveDistance(5.13);
    // wc.faceTarget({49.76, 49.99});
    // wc.addDistFn(3, []()
    //     { launchDisks(); });
    // wc.driveDistance(15.88);
};
Auton skills = "Skills" + []()
{
    wc.estimateStartPos(PVector(-61.52, 40.75), 90.72);
    skillsSpinRoller();
    intakeCount(3);
    chassis.setSpeedLimit(30);
    wc.driveTo(-43.19, 50.99);
    s(700);
    chassis.setSpeedLimit(100);
    wc.turnTo(0);
    wc.driveDistance(13.34);
    skillsFrontRoller();
    wc.backwardsDriveDistance(3.63);
    chassis.setSpeedLimit(50);
    wc.driveTo(12.07, 57.42);
    chassis.setSpeedLimit(100);
    wc.faceTarget({49.76, 50.82});
    s(300);
    launchDisks();
    s(500);
    intakeCount(3);
    chassis.setSpeedLimit(40);
    wc.followPath(&purePursuit, {PVector(-45.50, 55.61), PVector(19.21, 2.57)});
    wc.faceTarget({49.76, 50.82});
    wc.driveDistance(14);
    wc.faceTarget({49.76, 50.82});
    launchDisks();
    s(1000);
    chassis.setSpeedLimit(30);
    wc.driveTo(41.34, -25.2);
    chassis.setSpeedLimit(100);
    wc.driveTo(50.85, 19.78);
    wc.faceTarget({49.76, 50.82});
    launchDisks();
    wc.backInto(59.17, -41.14);
    wc.turnTo(270);
    wc.backwardsDriveDistance(1.53);
    skillsSpinRoller();
    intakeCount(3);
    wc.driveTo(32.59, -36.52);
    wc.backInto(34.40, -60.79);
    skillsSpinRoller();
    // wc.driveTo(-17.10, -57.82);
    // wc.faceTarget({-49.96, -50.22});
    // launchDisks();
    // intakeCount(3);
    // wc.followPath(&purePursuit, {PVector(44.64, -56.66), PVector(-19.58, -4.98)});
    // wc.faceTarget({-49.96, -50.22});
    // launchDisks();
    // wc.driveTo(-39.56, 14.66);
    // wc.driveTo(-55.41, -15.71);
    // wc.faceTarget({-49.96, -50.22});
    // wc.backInto(-51.45, 50.82);
    // wc.turnTo(135);
    // endgame.open();
};
Auton noOp = "No Run" + []() {};
Auton winPoint = "Win Point" + []()
{
    wc.estimateStartPos(PVector(-62.02, 41.24), 90.72);
    spinRoller();
    intakeCount(2);
    wc.followPath(&purePursuit, {PVector(-54.09, -7.62), PVector(-28.99, -0.03), PVector(-11.65, -12.08)});
    wc.faceTarget({49.26, 50.49});
    wc.backwardsDriveDistance(5.58);
    wc.addDistFn(3, []()
        { launchDisks(); });
    wc.driveDistance(15.62);
    wc.backwardsDriveDistance(3.03);
    intakeCount(2);
    wc.driveTo(-0.76, -23.31);
    wc.faceTarget({49.26, 50.49});
    wc.driveTo(39.85, -58.64);
    wc.turnTo(-0.43);
    wc.backwardsDriveDistance(2.35);
    spinRoller();
};
#endif
