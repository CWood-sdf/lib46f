#include "Flywheel/Controller.h"
#include <sstream>
#if BOT != 2
const int FlywheelDebugEl::size = sizeof(FlywheelDebugEl) / sizeof(double);
void makeKeyCont(lv_obj_t* key, const char* lbl, lv_color_t color, lv_coord_t distDown)
{
    // Create primary container and set padding
    lv_obj_t* keyCont = lv_obj_create(key);
    lv_obj_set_style_pad_all(keyCont, 0, 0);

    // Set container size
    lv_obj_set_width(keyCont, 70);
    lv_obj_set_height(keyCont, 20);

    // Make align and set color and position
    lv_obj_set_align(keyCont, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_opa(keyCont, 0, 0);
    lv_obj_set_style_border_opa(keyCont, 0, 0);
    lv_obj_set_pos(keyCont, 0, -distDown);

    // Make colored square
    lv_obj_t* squ = lv_obj_create(keyCont);
    lv_obj_set_size(squ, 15, 15);
    lv_obj_set_style_radius(squ, 03, 0);
    lv_obj_set_style_bg_color(squ, color, 0);
    lv_obj_set_align(squ, LV_ALIGN_LEFT_MID);
    lv_obj_set_style_border_opa(squ, 0, 0);
    lv_obj_set_scrollbar_mode(squ, LV_SCROLLBAR_MODE_OFF);

    // Make label
    lv_obj_t* label2 = lv_label_create(keyCont);
    lv_obj_set_align(label2, LV_ALIGN_LEFT_MID);
    lv_obj_set_pos(label2, 20, 0);
    lv_label_set_text(label2, lbl);
}

void basicGraph(bool remake, const char* text, FlywheelDebugEl out)
{
    static lv_obj_t* chart;
    static lv_chart_series_t* serTarg;
    static lv_chart_series_t* serMeas;
    static lv_chart_series_t* serFilt;
    static lv_chart_series_t* serErr;
    static lv_chart_series_t* serSent;
    static lv_chart_series_t* serDeriv;
    static lv_obj_t* chartLabel;
    static lv_obj_t* key;
    // cout << "LO" << endl;
    if (remake)
    {
        // cout << "LI" << endl;
        // //Make the chart
        chart = lv_chart_create(lv_scr_act());
        // cout << chart << endl;
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 6, 3, 11, 2, true, 40);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -100, 700);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_obj_set_size(chart, LV_HOR_RES_MAX - 40, LV_VER_RES_MAX - 10);
        lv_chart_set_point_count(chart, 200);
        lv_obj_center(chart);
        lv_obj_set_pos(chart, 20, 0);

        // Make the series
        serTarg = lv_chart_add_series(chart, lv_color_make(255, 200, 200), LV_CHART_AXIS_PRIMARY_Y);
        serMeas = lv_chart_add_series(chart, lv_color_make(255, 0, 0), LV_CHART_AXIS_PRIMARY_Y);
        serFilt = lv_chart_add_series(chart, lv_color_make(0, 0, 255), LV_CHART_AXIS_PRIMARY_Y);
        serErr = lv_chart_add_series(chart, lv_color_make(0, 255, 0), LV_CHART_AXIS_PRIMARY_Y);
        serSent = lv_chart_add_series(chart, lv_color_make(0, 255, 255), LV_CHART_AXIS_PRIMARY_Y);
        serDeriv = lv_chart_add_series(chart, lv_color_make(255, 150, 0), LV_CHART_AXIS_PRIMARY_Y);

        // Make the label
        chartLabel = lv_label_create(chart);
        lv_label_set_text(chartLabel, text);

        // Make the key bounding box
        key = lv_obj_create(chart);
        lv_obj_set_align(key, LV_ALIGN_BOTTOM_RIGHT);
        lv_obj_set_style_bg_color(key, lv_color_make(200, 200, 200), 0);
        lv_obj_set_style_pad_all(key, 0, 0);
        lv_obj_set_content_height(key, 120);
        lv_obj_set_content_width(key, 100);
        lv_obj_set_style_bg_opa(key, 0, 0);
        lv_obj_set_style_border_opa(key, 0, 0);

        // Make the key elements
        makeKeyCont(key, "Err", serErr->color, 0);
        makeKeyCont(key, "Targ", serTarg->color, 20);
        makeKeyCont(key, "meas", serMeas->color, 40);
        makeKeyCont(key, "Filt", serFilt->color, 60);
        makeKeyCont(key, "Sent", serSent->color, 80);
        makeKeyCont(key, "Deriv", serDeriv->color, 100);
        // cout << remake << endl;
        lv_chart_set_next_value(chart, serTarg, out.targetVel);
        lv_chart_set_next_value(chart, serErr, out.error);
        lv_chart_set_next_value(chart, serMeas, out.measuredVel);
        lv_chart_set_next_value(chart, serFilt, out.filterVel);
    }
    // cout << chart << endl;
    if (chart != NULL)
    {
        lv_label_set_text(chartLabel, text);
        lv_chart_set_next_value(chart, serTarg, out.targetVel);
        lv_chart_set_next_value(chart, serErr, out.error);
        lv_chart_set_next_value(chart, serMeas, out.measuredVel);
        lv_chart_set_next_value(chart, serFilt, out.filterVel);
        lv_chart_set_next_value(chart, serSent, out.sentVel * 6);
        lv_chart_set_next_value(chart, serDeriv, out.deriv);
    }
}
void FlywheelTBHEncoder::init()
{
    gain = 0.025;
    maxRateGain = 10;
    maxRateDrop = 10;
    velCheck = Settled(10, 1000, 500);
}
void FlywheelTBHEncoder::setDisabled(bool p)
{
    disabled = p;
}
void FlywheelTBHEncoder::graph(bool remake)
{
    static char text[80] = "TBHE ctrl";
    const char* newName = "TBHE ctrl - ";
    std::stringstream thing = std::stringstream();
    thing << debug.targetVel / 6;
    const char* otherName = thing.str().c_str();
    // cout << otherName << endl;
    int i = 0;
    for (; newName[i] != '\0'; i++)
    {
        text[i] = newName[i];
    }
    for (int j = 0; otherName[j] != '\0'; j++)
    {
        text[i] = otherName[j];
        i++;
    }
    text[i] = '\0';
    basicGraph(remake, text, debug);
}
FlywheelTBHEncoder::FlywheelTBHEncoder(MotorGroup& m, Encoder p) : mots(m), filter(0.7), minMaxFilter(4), weightFilter(4, 2.0, 0), sma(5)
{
    init();
    filter.seed(0);
    en = p;
}
FlywheelTBHEncoder::FlywheelTBHEncoder(MotorGroup& m) : FlywheelTBHEncoder(m, Encoder(m[0]))
{
}

void FlywheelTBHEncoder::setTarget(int i)
{
    if (velTargets.size() == 0)
        return;
    target = i;
    if (target < 0)
    {
        target = 0;
    }
    else if (target >= velTargets.size())
    {
        target = velTargets.size() - 1;
    }

    tbh = initialTbh[target];
    if (velTargets.size() != 0)
    {
        hasTarget = true;
    }
}
void FlywheelTBHEncoder::setTargetSpeed(double t)
{
    // Loop through the targets and check if t is already in it
    for (int i = 0; i < velTargets.size(); i++)
    {
        if (velTargets[i] == t)
        {
            setTarget(i);
            return;
        }
    }
    // If it isn't, add it
    addTarget(t);
    setTarget(velTargets.size() - 1);
}
void FlywheelTBHEncoder::addTarget(double t)
{
    velTargets.push_back(t);
    initialTbh.push_back(t / 6.0);
}
bool FlywheelTBHEncoder::ready()
{
    return velCheck.settled();
}
void FlywheelTBHEncoder::step()
{
    if (!hasTarget)
        return;
    static LinkedList<double> lastVals = {0, 1, 5, 10, 2, 6, 10};
    auto removeExtremes = [](LinkedList<double> list)
    {
        double min = list.getBase();
        double max = list.getBase();
        for (auto val : list)
        {
            if (val < min)
            {
                min = val;
            }
            if (val > max)
            {
                max = val;
            }
        }
        bool removeMin = false;
        bool removeMax = false;
        for (auto val : list)
        {
            if (val == min && !removeMin)
            {
                removeMin = true;
                list.popCurrent();
            }
            if (val == max && !removeMax)
            {
                removeMax = true;
                list.popCurrent();
            }
            // break on both removed
            if (removeMin && removeMax)
            {
                break;
            }
        }
        return list;
    };
    // A lot of variables
    static double lastRotation = 0;
    static double lastVel = 0.0;
    static double lastDesiredVel = 0;
    static double prevErr = 0;
    static int settledCount = 0;
    double velSent = 10;
    bool calcTbh = true;
    double desiredVel = velTargets[target];
    // The interval between the last step and this one
    int timeStep = velCheck.timeStep();
    double rotation = en.position(rev);
    // An estimate of the velocity
    double speedEst = abs(rotation - lastRotation) / max((double)timeStep, 1.0) * 60.0 * 1000.0;
    // speedEst = (speedEst + lastEst) / 2.0;
    lastRotation = rotation;
    // Get a filtered velocity
    lastVals.pushBack(speedEst);
    while (lastVals.size() > 8)
    {
        lastVals.popBase();
    }
    auto list = removeExtremes(removeExtremes(removeExtremes(lastVals)));
    // Average
    double sum = 0;
    for (auto val : list)
    {
        sum += val;
    }
    double extremeRemoved = sum / list.size();
    sma.update(extremeRemoved);
    minMaxFilter.update(sma);
    weightFilter.update(sma);
    filter.update(weightFilter);
    double speed = filter;
    double err = desiredVel - speed;
    // Check if the flywheel speed is stable and if the error is small
    bool settled = velCheck.settled(err);
    static bool freeAccel = false;

    if (settled)
    {
        calcTbh = false;
        // settledCount++;
        // if(settledCount > 1000/50){
        //   settledCount = 1000/50;
        // }
    }
    // else {
    //   settledCount--;
    //   if(settledCount > 0){
    //     calcTbh = false;
    //   }
    // }
    // If the target velocity has changed, don't recalculate the tbh
    if (desiredVel != lastDesiredVel)
    {
        calcTbh = false;
        settledCount = 0;
    }
    lastDesiredVel = desiredVel;
    // Vary the gain value to optimize the flywheel acceleration
    if (abs(err) < 10)
    {
        gain = 0.0002;
    }
    if (abs(err) < 40)
    {
        gain = 0.001;
    }
    else
    {
        gain = 0.004;
    }
    if (calcTbh)
    {
        if (std::signbit(err) != std::signbit(prevErr))
        {
            tbh = (lastVel + tbh) / 2;
            if (tbh < 0)
            {
                tbh = 0;
            }
            if (tbh > 100)
            {
                tbh = 100;
            }
            initialTbh[target] = tbh;
            velSent = tbh;
        }
        else
        {
            velSent = lastVel + gain * err;
        }
    }
    else
    {
        velSent = lastVel;
    }
    // cout << velSent << endl;
    // cout << (abs(err) < 200) << ", " << abs(velSent - lastVel) << ", " << (abs(velSent - lastVel) < 4.0) << ", " << !settled << endl;

    // cout << signbit(err) << endl;
    // cout << velSent << endl << endl;

    // Limit the velocity change
    if (velSent - lastVel > maxRateGain)
    {
        velSent = lastVel + maxRateGain;
    }
    else if (velSent - lastVel < -maxRateDrop)
    {
        velSent = lastVel - maxRateDrop;
    }
    // //Slow down the acceleration if the flywheel is close to the target
    // if(abs(err) < 60 && velSent - lastVel > 0.1){
    //   velSent = lastVel + 0.1;
    // }
    if (velSent < 0)
    {
        velSent = 0;
    }
    if (velSent > 100)
    {
        velSent = 100;
    }
    if (abs(err) > 100 && speed < desiredVel)
    {
        freeAccel = true;
    }
    else
    {
        freeAccel = false;
    }
    if (freeAccel)
    {
        if (speedEst < desiredVel)
        {
            velSent = 100;
        }
        else
        {
            // velSent = initialTbh[target];
            freeAccel = false;
        }
    }
    debug.set(err, speedEst, speed, desiredVel, velSent, velCheck.getDeriv() * 100.0);
    prevErr = err;
    if (!disabled)
    {
        mots.spinVolt(fwd, velSent);
    }
    lastVel = velSent;
    // s(5);
}
#endif
