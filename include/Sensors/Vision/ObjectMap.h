#include "FieldObject.h"
#include "Filters.h"
bool toBool(double v);
struct BasicObject
{
    typedef BasicEMA<bool, double, toBool> BoolEMA;
    BasicEMA<PVector> origin = BasicEMA<PVector>(0.9);
    EMA width = EMA(0.95); // Give 95% trust to old measurements in case of object splitting
    EMA height = EMA(0.95);
    BoolEMA exist = BoolEMA(0.7, true); // Object should delete self pretty quickly
    BoolEMA split = BoolEMA(0.7, false);
    void addData(vision::object& object)
    {
        origin.update({static_cast<double>(object.originX), static_cast<double>(object.originY)});
        width.update(object.width);
        height.update(object.height);
    }
    bool contains(vision::object& object)
    {
        return false;
    }
    BasicObject()
    {
        origin.seed({0, 0});
        width.seed(0);
        height.seed(0);
    }
    BasicObject(vision::object& start)
    {
        origin.seed({static_cast<double>(start.originX), static_cast<double>(start.originY)});
        width.seed(start.width);
        height.seed(start.height);
    }
    BasicObject(const BasicObject&) = default;
    BasicObject(BasicObject&&) = default;
    BasicObject& operator=(const BasicObject&) = default;
};
template <class Tp, class Friend>
class Array2
{
    friend Friend;
    vector<Tp> arr;

public:
    int size()
    {
        return arr.size();
    }
    Tp& operator[](size_t key)
    {
        return arr[key];
    }
};
struct ObjectMap
{
    CircFieldObject object;
    vision& sensor;
    Array2<BasicObject, ObjectMap> objects;
    void update()
    {
        // Need non-existent object popping (prolly build new safearray)
        sensor.takeSnapshot(*object.color);
        vector<vision::object> arr;
        for (int i = 0; i < sensor.objects.getLength(); i++)
        {
            arr.push_back(sensor.objects[i]);
        }
        for (int i = 0; i < objects.size(); i++)
        {
            // vision::object* closestObj = NULL;
            PVector topLeft = objects[i].origin.value() - PVector(10, 10);
            PVector btmRight = objects[i].origin.value() + PVector(objects[i].width, objects[i].height) + PVector(10, 10);

            PVector objOrigin;
            PVector objWH;
            double shortestDist = 400.0;
            int objIndex = 0;
            // Find nearest object (by top left corner)
            for (int j = 0; j < arr.size(); j++)
            {
                PVector origin = PVector(arr[j].originX, arr[j].originY);
                double oldShortest = shortestDist;
                if (origin.dist2D(topLeft) < shortestDist + 3)
                {
                    shortestDist = origin.dist2D(topLeft);
                }
                if (
                    abs(shortestDist - oldShortest) < 5 &&
                    arr[j].originX > topLeft.x &&
                    arr[j].originY > topLeft.y &&
                    arr[j].originX + arr[j].width < btmRight.x &&
                    arr[j].originY + arr[j].height < btmRight.y)
                {
                    // Grab new object
                    objIndex = j;
                    objOrigin = origin;
                    objWH = PVector(arr[j].width, arr[j].height);
                }
                else
                {
                    shortestDist = oldShortest;
                }
            }
            if (shortestDist < 10)
            {
                objects[i].exist.update(true);
                objects[i].origin.update(objOrigin);
                objects[i].height.update(objWH.y);
                objects[i].width.update(objWH.x);
                arr.erase(arr.begin() + objIndex);
            }
            else
            {
                objects[i].exist.update(false);
            }
            if (!objects[i].exist)
            {
                objects.arr.erase(objects.arr.begin() + i);
                i--;
            }
        }
        for (auto& obj : arr)
        {
            objects.arr.push_back(obj);
        }
    }
};
