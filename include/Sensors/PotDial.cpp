#include "Sensors/PotDial.h"

void SelectorArr::addDial(PotDial* dial)
{
    for (auto p : boundDials)
    {
        // Dial already contained in the array, abort
        if (p == dial)
        {
            return;
        }
    }
    boundDials.push_back(dial);
    if (toVal.size() > 0)
    {
        cout << "Warning in SelectorArr: "
             << "Adding new dial which could potentially mess up current " << endl;
        toVal.clear();
    }
}
SelectorArr::SelectorArr(vector<int> count, FnTp fn)
{
    if (count.size() != boundDials.size())
    {
        cout
            << "Warning in SelectorArr: Invalid input size, given array size ("
            << count.size()
            << ") does not match dial count of ("
            << boundDials.size()
            << ")"
            << endl;
    }
    toVal[count] = fn;
    this->count = count;
}
SelectorArr::SelectorArr(vector<int> count)
{
    if (count.size() != boundDials.size())
    {
        cout
            << "Warning in SelectorArr: Invalid input size, given array size ("
            << count.size()
            << ") does not match dial count of ("
            << boundDials.size()
            << ")"
            << endl;
    }
    toVal[count] = emptyFn;
}
void SelectorArr::attachFn(FnTp fn)
{
    toVal[count] = fn;
}

int PotDial::getAmnt()
{
    double angle = sensor->angle(deg);
    if (angle <= baseVal)
    {
        return 1;
    }
    else if (angle >= baseVal + range)
    {
        return ticks;
    }
    else
    {
        angle -= baseVal;
        angle /= (double)range;
        angle *= (double)ticks - 1;
        return round(angle) + 1;
    }
}
SelectorArr::FnTp SelectorArr::getVal()
{
    vector<int> vals;
    for (auto l : boundDials)
    {
        vals.push_back(l->getAmnt());
    }
    if (toVal.count(vals) == 1)
    {
        return toVal[vals];
    }
    // Err msg: show inputs received by prog, show possible inputs
    cout << "In SelectorArr: ";
    cout << "Input array of {";
    for (int i = 0; i < vals.size(); i++)
    {
        cout << vals[i];
        if (i != vals.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "} does not match any of the declared inputs: \n";
    for (auto& [key, val] : toVal)
    {
        cout << "  {";
        for (int i = 0; i < key.size(); i++)
        {
            cout << key[i];
            if (i != key.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << "}\n";
    }
    cout << flush;
    return emptyFn;
}
