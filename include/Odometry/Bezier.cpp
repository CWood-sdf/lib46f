#include "Bezier.h"
#include "EPA_Tracker.h"
// Constructor
VectorArr::VectorArr(std::initializer_list<PVector> list)
{
    for (PVector v : list)
    {
        arr.push_back(v);
    }
}
// Use decltype because I don't care enough to find out the type

// Iterators:
decltype(VectorArr::arr.begin()) VectorArr::begin()
{
    return arr.begin();
}
decltype(VectorArr::arr.end()) VectorArr::end()
{
    return arr.end();
}
// Bracket access
PVector& VectorArr::operator[](int i)
{
    return arr[i];
}
void VectorArr::pop(int i)
{
    arr.erase(arr.cbegin() + i, arr.cbegin() + i + 1);
}
// void popCurrentNext(){
//   arr.popCurrentNext();
// }
void VectorArr::popBase()
{
    arr.erase(arr.cbegin(), arr.cbegin() + 1);
}
void VectorArr::popEnd()
{
    arr.erase(arr.cbegin() + arr.size() - 1, arr.cbegin() + arr.size());
}
// Get the size
size_t VectorArr::size()
{
    return arr.size();
}
// Add elements
void VectorArr::push(PVector v)
{
    arr.push_back(v);
}
void VectorArr::pushBase(PVector v)
{
    arr.insert(arr.cbegin(), v);
}

// Get last / first elements
PVector& VectorArr::last()
{
    return arr.back();
}
PVector& VectorArr::first()
{
    return arr.front();
}

// operator=
VectorArr& VectorArr::operator=(const VectorArr& v)
{
    arr = v.arr;
    return *this;
}
// operator=
VectorArr& VectorArr::operator=(VectorArr&& v)
{
    arr = v.arr;
    return *this;
}
double VectorArr::getCurveLength()
{
    double length = 0;
    for (int i = 0; i < arr.size() - 1; i++)
    {
        length += arr[i].dist2D(arr[i + 1]);
    }
    return length;
}
// Find a single point on a bezier curve with parameter t (t goes from 0 -> 1)
PVector bezierInterpolate(VectorArr ptArr, double t)
{
    // The array of the interpolated points
    VectorArr newPts = {};
    for (int i = 0; i < ptArr.size() - 1; i++)
    {
        // Interpolate between current point and next
        PVector newPt = ptArr[i + 1] - ptArr[i];
        newPt *= t;
        newPt += ptArr[i];
        newPts.push(newPt);
    }
    // If interpolated point array still has multiple elements
    if (newPts.size() >= 2)
    {
        // YAYYY RECURSION!!!!
        return bezierInterpolate(newPts, t);
    }
    else
    {
        // Otherwise return the only element
        return newPts.first();
    }
}
// Create a bezier curve
VectorArr bezierCurve(VectorArr ptArr, double inc)
{
    // double spacing = 1.0;
    // Return value
    VectorArr ret;
    // Go through multiple rounds of interpolation
    for (double i = 0; i <= 1; i += inc)
    {
        PVector pt = bezierInterpolate(ptArr, i);
        ret.push(pt);
    }
    ret.push(ptArr.last());
    return ret;
}
class AutoDiff
{
    double value = 0;
    double derivative = 0;

public:
    AutoDiff(double value, double derivative = 0) : value(value), derivative(derivative) {}
    AutoDiff() {}
    AutoDiff operator+(const AutoDiff& other) const
    {
        return AutoDiff(value + other.value, derivative + other.derivative);
    }
    AutoDiff operator-(const AutoDiff& other) const
    {
        return AutoDiff(value - other.value, derivative - other.derivative);
    }
    AutoDiff operator*(const AutoDiff& other) const
    {
        return AutoDiff(value * other.value, derivative * other.value + value * other.derivative);
    }
    AutoDiff& operator=(const AutoDiff& other)
    {
        value = other.value;
        derivative = other.derivative;
        return *this;
    }
    static AutoDiff constant(double value)
    {
        return AutoDiff(value, 0);
    }
    static AutoDiff variable(double value)
    {
        return AutoDiff(value, 1);
    }
    double getValue() const
    {
        return value;
    }
    double getDerivative() const
    {
        return derivative;
    }
};
AutoDiff bezierPartialLerp(vector<AutoDiff> arr, AutoDiff t)
{
    if (arr.size() == 1)
    {
        return arr[0];
    }
    vector<AutoDiff> newPts;
    for (int i = 0; i < arr.size() - 1; i++)
    {
        AutoDiff newPt = arr[i + 1] - arr[i];
        newPt = newPt * t;
        newPt = newPt + arr[i];
        newPts.push_back(newPt);
    }
    return bezierPartialLerp(newPts, t);
}
AutoDiff getBezierDeivativeMult(int size, int index, double t)
{
    vector<AutoDiff> arr = {};
    arr.resize(size);
    for (int i = 0; i < size; i++)
    {
        if (i == index)
        {
            arr[i] = AutoDiff::constant(1);
        }
        else
        {
            arr[i] = AutoDiff::constant(0);
        }
    }
    return bezierPartialLerp(arr, AutoDiff::variable(t));
}
VectorArr bezierDerivative(VectorArr ptArr, double inc)
{
    VectorArr ret;
    for (double i = 0.000; i <= 1.0 + 1e-9; i += inc)
    {
        PVector deriv = {0, 0, 0};
        for (int j = 0; j < ptArr.size(); j++)
        {
            AutoDiff mult = getBezierDeivativeMult(ptArr.size(), j, i);
            PVector pt = ptArr[j];
            pt *= mult.getDerivative();
            deriv += pt;
        }
        ret.push(deriv);
    }
    return ret;
}
VectorArr bezierAcc(VectorArr ptArr, double inc)
{
    VectorArr derivative = bezierDerivative(ptArr, inc);
    VectorArr acc;
    for (int i = 0; i < derivative.size() - 1; i++)
    {
        acc.push(derivative[i + 1] - derivative[i]);
    }
    acc.push(acc.last());
    return acc;
}
pair<VectorArr, VectorArr> bezierCurveNormalLR(VectorArr ptArr, double dist, double inc)
{
    // Return value
    pair<VectorArr, VectorArr> ret;
    VectorArr bezier = bezierCurve(ptArr, inc);

    VectorArr derivative = bezierDerivative(ptArr, inc);
    for (auto& i : derivative)
    {
        i.normalize();
    }
    for (int i = 0; i < derivative.size(); i++)
    {
        ret.first.push(
            derivative[i]
                .get()
                .rotate(-90)
                .mult(dist)
                .add(bezier[i]));
        ret.second.push(
            derivative[i]
                .get()
                .rotate(90)
                .mult(dist)
                .add(bezier[i]));
    }
    return ret;
}

vector<double> bezierCurvature(VectorArr ptArr, double inc)
{
    vector<double> ret;
    auto curve = bezierCurve(ptArr, inc);
    ret.push_back(0.0000001);
    for (int i = 1; i < curve.size() - 1; i++)
    {
        auto p = curve[i];
        auto q = curve[i - 1];
        auto r = curve[i + 1];
        double y1 = p.y;
        double x1 = p.x;
        double x2 = q.x;
        double y2 = q.y;
        double x3 = r.x;
        double y3 = r.y;
        double k1 = 0.5 * (pow(x1, 2) + pow(y1, 2) - pow(x2, 2) - pow(y2, 2)) / (x1 - x2 + 0.0000001);
        double k2 = (y1 - y2) / (x1 - x2 + 0.0000001);
        double b = 0.5 * (pow(x2, 2) - 2.0 * x2 * k1 + pow(y2, 2) - pow(x3, 2) + 2.0 * x3 * k1 - pow(y3, 2)) / (x3 * k2 - y3 + y2 - x2 * k2 + 0.0000001);
        double a = k1 - k2 * b;
        double rad = sqrt(pow(x1 - a, 2) + pow(y1 - b, 2));
        ret.push_back(1.0 / rad);
    }
    ret.push_back(0.00000001);
    // VectorArr derivative = bezierDerivative(ptArr, inc);
    // VectorArr acc = bezierAcc(ptArr, inc);
    // for(int i = 0; i < derivative.size(); i++){
    //   Matrix<double, 2, 2> combo;
    //   combo[0][0] = derivative[i].x;
    //   combo[0][1] = derivative[i].y;
    //   combo[1][0] = acc[i].x;
    //   combo[1][1] = acc[i].y;
    //   double denominator = pow(derivative[i].mag(), 3);
    //   if(denominator != 0)
    //     ret.push_back(combo.determinant() / denominator);
    //   else
    //     ret.push_back(1e99);
    // }
    return ret;
}
