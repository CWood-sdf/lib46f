#include "BotTracker.h"
#include "Odometry/EPA_Wheel_Control.h"
std::vector<lv_point_t> pts = {{0, 0}, {100, 100}, {50, 100}, {150, 150}};
std::vector<lv_point_t> actualPts;
std::vector<lv_point_t> pathPts = {};
extern Chassis chassis;
extern WheelController wc;
void displayBot(bool remake)
{
    static lv_obj_t* botContainer;
    static lv_obj_t* line2;
    static lv_obj_t* centerPt;
    static lv_obj_t* labelX;
    static lv_obj_t* labelY;
    static lv_obj_t* labelA;
    static lv_obj_t* pathLine;
    if (remake)
    {
        short gridHeight = LV_VER_RES;
        short sixthHeight = (LV_VER_RES / 6);
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(50, 50, 50), 0);
        lv_obj_t* gridContainer = lv_obj_create(lv_scr_act());
        lv_obj_set_style_pad_all(lv_scr_act(), 0, 0);
        lv_obj_set_style_bg_opa(gridContainer, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(gridContainer, LV_OPA_TRANSP, 0);
        lv_obj_set_size(gridContainer, gridHeight + 1, gridHeight + 1);
        lv_obj_set_style_pad_all(gridContainer, -2, 0);
        lv_obj_set_pos(gridContainer, 40, 0);

        pts.clear();
        for (int i = 1; i < 6; i++)
        {
            if (i % 2 == 0)
            {
                pts.push_back({(short)(sixthHeight * (i)), (short)0});
                pts.push_back({(short)(sixthHeight * (i)), gridHeight});
            }
            else
            {
                pts.push_back({(short)(sixthHeight * (i)), gridHeight});
                pts.push_back({(short)(sixthHeight * (i)), (short)0});
            }
        }
        pts.push_back({gridHeight, (short)0});
        for (int i = 1; i < 6; i++)
        {
            if (i % 2 == 0)
            {
                pts.push_back({(short)0, (short)(sixthHeight * (i))});
                pts.push_back({gridHeight, (short)(sixthHeight * (i))});
            }
            else
            {
                pts.push_back({gridHeight, (short)(sixthHeight * (i))});
                pts.push_back({(short)0, (short)(sixthHeight * (i))});
            }
        }
        pts.push_back({(short)0, (short)0});
        pts.push_back({gridHeight, (short)0});
        pts.push_back({gridHeight, gridHeight});
        pts.push_back({(short)0, gridHeight});

        lv_obj_t* line = lv_line_create(gridContainer);
        lv_line_set_points(line, pts.data(), pts.size());
        // lv_obj_add_style(line, &style_title, 0);
        lv_obj_set_style_line_width(line, 1, 0);
        lv_obj_set_style_line_rounded(line, true, 0);
        {
            auto basicAlign = [](lv_obj_t* obj, int y)
            {
                lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                lv_obj_align(obj, LV_ALIGN_TOP_LEFT, 300, y);
            };
            // lv_obj_t* labelX;
            labelX = lv_label_create(lv_scr_act());
            basicAlign(labelX, 5);
            std::string setTextX = "x: ";
            setTextX += "3";
            lv_label_set_text(labelX, setTextX.data());

            // lv_obj_t* labelY;
            labelY = lv_label_create(lv_scr_act());
            basicAlign(labelY, 25);
            std::string setTextY = "y: ";
            setTextY += "3.1415936539";
            lv_label_set_text(labelY, setTextY.data());

            // lv_obj_t* labelA;
            labelA = lv_label_create(lv_scr_act());
            std::string setTextA = "a: ";
            setTextA += "3";
            lv_label_set_text(labelA, setTextA.data());
            basicAlign(labelA, 45);
        }
        double lvObjWidth = 0.6;
        double dSixthHeight = static_cast<double>(sixthHeight);
        lv_obj_t* redGoal = lv_obj_create(gridContainer);
        lv_obj_align(redGoal, LV_ALIGN_TOP_LEFT, (int)(dSixthHeight * (5.4 - lvObjWidth / 2)), (int)(dSixthHeight * (0.6 - lvObjWidth / 2)));
        lv_obj_set_size(redGoal, dSixthHeight * lvObjWidth, dSixthHeight * lvObjWidth);
        lv_obj_set_style_bg_color(redGoal, lv_color_make(255, 0, 0), 0);
        lv_obj_set_style_border_width(redGoal, 0, 0);
        lv_obj_set_style_border_color(redGoal, lv_color_make(0, 0, 0), 0);
        lv_obj_set_style_radius(redGoal, dSixthHeight * lvObjWidth, 0);

        lv_obj_t* blueGoal = lv_obj_create(gridContainer);
        lv_obj_align(blueGoal, LV_ALIGN_TOP_LEFT, dSixthHeight * (0.6 - lvObjWidth / 2), dSixthHeight * (5.4 - lvObjWidth / 2));
        lv_obj_set_size(blueGoal, dSixthHeight * lvObjWidth, dSixthHeight * lvObjWidth);
        lv_obj_set_style_bg_color(blueGoal, lv_color_make(0, 0, 255), 0);
        lv_obj_set_style_border_width(blueGoal, 0, 0);
        lv_obj_set_style_border_color(blueGoal, lv_color_make(0, 0, 0), 0);
        lv_obj_set_style_radius(blueGoal, dSixthHeight * lvObjWidth, 0);

        botContainer = lv_obj_create(gridContainer);
        line2 = lv_line_create(botContainer);
        pathLine = lv_line_create(gridContainer);
        centerPt = lv_obj_create(botContainer);

        lv_obj_set_style_line_width(line2, 3, 0);
        lv_obj_set_style_line_rounded(line2, true, 0);
        lv_obj_set_style_line_color(line2, lv_color_make(0, 0, 0), 0);
        lv_obj_set_style_line_opa(line2, LV_OPA_MAX, 0);

        lv_obj_set_style_line_width(pathLine, 3, 0);
        lv_obj_set_style_line_rounded(pathLine, true, 0);
        lv_obj_set_style_line_color(pathLine, lv_color_make(0, 0, 0), 0);
        lv_obj_set_style_line_opa(pathLine, LV_OPA_MAX, 0);

        lv_obj_set_style_bg_opa(botContainer, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(botContainer, LV_OPA_TRANSP, 0);
        lv_obj_set_style_pad_all(botContainer, 0, 0);

        lv_obj_set_align(centerPt, LV_ALIGN_CENTER);
        lv_obj_set_style_radius(centerPt, 5, 0);
        lv_obj_set_size(centerPt, 5, 5);
    }

    PVector pos = chassis.botPos();
    double angle = chassis.botAngle();

    short height = LV_VER_RES;

    // Zero position
    /*pos.x += 72;
    pos.y += 72;*/

    pos *= height / 144.0;

    double width = 10;
    double halfContWidth = lv_obj_get_width(botContainer) / 2 - 3;
    std::vector<PVector> lineCorners = {
        {-width, width},
        {-width, -width},
        {width, -width},
        {width, width}};
    actualPts.clear();
    for (PVector& v : lineCorners)
    {
        v.rotate(angle);
        actualPts.push_back(v + PVector(halfContWidth, halfContWidth));
    }
    lv_line_set_points(line2, actualPts.data(), actualPts.size());

    if (wc.drawArr)
    {
        auto p = wc.publicPath;
        pathPts.clear();
        for (auto pt : p)
        {

            pt += {72, 72};
            pt *= height / 144.0;
            pathPts.push_back(pt);
        }
    }
    lv_line_set_points(pathLine, pathPts.data(), pathPts.size());

    lv_obj_align(botContainer, LV_ALIGN_CENTER, pos.x, pos.y);
    lv_obj_set_size(botContainer, width * M_SQRT2 + 30, width * M_SQRT2 + 30);
    std::string setTextY = "y: ";
    setTextY += toCcp(chassis.botPos().y);
    lv_label_set_text(labelY, setTextY.data());

    std::string setTextA = "a: ";
    setTextA += toCcp(angle);
    lv_label_set_text(labelA, setTextA.data());

    std::string setTextX = "x: ";
    setTextX += toCcp(chassis.botPos().x);
    lv_label_set_text(labelX, setTextX.data());
}
