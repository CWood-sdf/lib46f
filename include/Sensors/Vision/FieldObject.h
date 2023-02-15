#ifndef FIELD_OBJECT_H
#define FIELD_OBJECT_H
#include "vex.h"
struct CircFieldObject
{
    vision::signature* color;
    double groundToMid = 0;
    vector<PVector> topLeftBound;
    vector<PVector> btmRghtBound;
    double getDistEst(double sensorHeight, double distEst, PVector pt)
    {
        double dFront = distEst / sensorHeight * (pt.y + groundToMid);
        return distEst - dFront + pt.x;
    }
    double getCenterObjectVertEstimateTop(double topEst, double sensorHeight)
    {
        double angle = atan(sensorHeight / topEst); // in rad
        if (topLeftBound.size() != 1)
        {
            int i = 0;
            while (90 - topLeftBound[i].angleTo(topLeftBound[i + 1]) > angle)
            {
                if (i == topLeftBound.size() - 1)
                {
                    break;
                }
            }
            if (i != 0 && abs(90 - topLeftBound[i].angleTo(topLeftBound[i + 1]) - angle) > abs(90 - topLeftBound[i - 1].angleTo(topLeftBound[i]) - angle))
            {
                // Use btm
                i--;
            }
            if (90 - topLeftBound[i].angleTo(topLeftBound[i + 1]) < angle)
            {
                // Change to top
                i++;
            }
            auto pt = topLeftBound[i];
            return getDistEst(sensorHeight, topEst, pt);
        }
        else
        {
            auto pt = topLeftBound[0];
            return getDistEst(sensorHeight, topEst, pt);
        }
    }
    double getCenterObjectVertEstimateBtm(double btmEst, double sensorHeight)
    {
        double angle = atan(sensorHeight / btmEst); // in rad
        if (btmRghtBound.size() != 1)
        {
            int i = 0;
            while (90 - btmRghtBound[i].angleTo(btmRghtBound[i + 1]) < angle)
            {
                if (i == btmRghtBound.size() - 1)
                {
                    break;
                }
            }
            if (i != 0 && abs(90 - btmRghtBound[i].angleTo(btmRghtBound[i + 1]) - angle) < abs(90 - btmRghtBound[i - 1].angleTo(btmRghtBound[i]) - angle))
            {
                // Use btm
                i--;
            }
            if (90 - btmRghtBound[i].angleTo(btmRghtBound[i + 1]) > angle)
            {
                // Change to top
                i++;
            }
            auto pt = btmRghtBound[i];
            return getDistEst(sensorHeight, btmEst, pt);
        }
        else
        {
            auto pt = btmRghtBound[0];
            return getDistEst(sensorHeight, btmEst, pt);
        }
    }
    PVector getCenterObjectHorEstimateLeft(double est, double sensorHeight, double distEst, double angleFromSensor)
    {
        PVector farthestPt = {0, 0};
        for (auto i : topLeftBound)
        {
            if (abs(i.x) > abs(farthestPt.x))
            {
                farthestPt = i;
            }
        }
        farthestPt.rotateXZ(-angleFromSensor);
        double dist = PVector(est, distEst).mag();
        // double hypotenuse = PVector(dist, sensorHeight).mag();
        double dFront = dist / sensorHeight * (farthestPt.y + groundToMid);
        PVector endPt = {est, distEst};
        PVector linePt = PVector(dFront, 0).rotate(endPt.heading() + 180);
        endPt += linePt;
        PVector posEst = endPt + PVector(farthestPt.x, farthestPt.z);
        return posEst;
    }
    PVector getCenterObjectHorEstimateRight(double est, double sensorHeight, double distEst, double angleFromSensor)
    {
        PVector farthestPt = {0, 0};
        for (auto i : btmRghtBound)
        {
            if (abs(i.x) > abs(farthestPt.x))
            {
                farthestPt = i;
            }
        }
        farthestPt.rotateXZ(angleFromSensor);
        double dist = PVector(est, distEst).mag();
        // double hypotenuse = PVector(dist, sensorHeight).mag();
        double dFront = dist / sensorHeight * (farthestPt.y + groundToMid);
        PVector endPt = {est, distEst};
        PVector linePt = PVector(dFront, 0).rotate(endPt.heading() + 180);
        endPt += linePt;
        PVector posEst = endPt + PVector(farthestPt.x, farthestPt.z);
        return posEst;
    }
    PVector getRelPosLocation(PVector topLeft, PVector btmRight, double sensorHeight, double leftAngle, double rightAngle)
    {
        double yEstTop = getCenterObjectVertEstimateTop(topLeft.y, sensorHeight);
        double yEstBtm = getCenterObjectVertEstimateBtm(btmRight.y, sensorHeight);
        double yEst = (yEstBtm + yEstTop) / 2.0;
        PVector leftEst = getCenterObjectHorEstimateLeft(topLeft.x, sensorHeight, yEst, leftAngle);
        PVector rightEst = getCenterObjectHorEstimateRight(btmRight.x, sensorHeight, yEst, rightAngle);
        PVector estTotal = PVector(
            (leftEst.x + rightEst.x) / 2.0,
            (leftEst.y + rightEst.y + yEst) / 3.0 // May want to weight yEst:
                                                  //    ... + yEst * 2.0) / 4.0
        );
        return estTotal;
    }
};

#endif
