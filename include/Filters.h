#ifndef EMA_FILTER_H
#define EMA_FILTER_H
#include "vex.h"
template <class G, class T>
G converter_def(T val)
{
    return val;
}
template <class Get_T, class T = Get_T, Get_T (*converter)(T) = converter_def>
class BasicEMA
{
    double alpha;
    T val;
    T oldVal;

public:
    BasicEMA(double alpha)
    {
        this->alpha = alpha;
    }
    BasicEMA(double alpha, T val) : BasicEMA<Get_T, T, converter>(alpha)
    {
        seed(val);
    }
    void update(T sensorVal)
    {
        val = sensorVal * alpha + oldVal * (1 - alpha);
        oldVal = val;
    }
    T& valRef()
    {
        return val;
    }
    Get_T value()
    {
        return converter(val);
    }
    operator Get_T()
    {
        return value();
    }
    void seed(T value)
    {
        val = value;
    }
    BasicEMA<Get_T>& operator=(const BasicEMA<Get_T>& input)
    {
        val = input.val;
        alpha = input.alpha;
        return *this;
    }
};
class EMA : public BasicEMA<double>
{
public:
    using BasicEMA<double>::value;
    using BasicEMA<double>::update;
    using BasicEMA<double>::valRef;
    using BasicEMA<double>::operator double;
    using BasicEMA<double>::seed;
    using BasicEMA<double>::operator=;
    EMA(double a) : BasicEMA<double>(a)
    {
    }
    EMA(double a, double v) : BasicEMA<double>(a, v)
    {
    }
};

class SMA
{
    size_t size;
    LinkedList<double> vals = {};

public:
    SMA(size_t size)
    {
        this->size = size;
    }
    SMA(size_t size, double val) : SMA(size)
    {
        seed(val);
    }
    void update(double sensorVal)
    {
        vals.pushBack(sensorVal);
        while (vals.size() > size)
        {
            vals.popBase();
        }
    }
    double value()
    {
        double sum = 0;
        for (auto& val : vals)
        {
            sum += val;
        }
        return sum / vals.size();
    }
    operator double()
    {
        return value();
    }
    void seed(double value)
    {
        vals.clear();
        vals.pushBack(value);
    }
    SMA& operator=(const SMA& input)
    {
        size = input.size;
        vals = input.vals;
        return *this;
    }
};
// A filter that just averages the extreme values of the data
class MinMaxFilter
{
    // The number of values to select from
    size_t size;
    // The values
    LinkedList<double> vals = {};

public:
    MinMaxFilter(size_t size)
    {
        this->size = size;
    }
    MinMaxFilter(size_t size, double val) : MinMaxFilter(size)
    {
        seed(val);
    }
    void update(double sensorVal)
    {
        vals.pushBack(sensorVal);
        while (vals.size() > size)
        {
            vals.popBase();
        }
    }
    double value()
    {
        double min = vals.getBase();
        double max = vals.getBase();
        for (auto& val : vals)
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
        return (min + max) / 2;
    }
    operator double()
    {
        return value();
    }
    void seed(double value)
    {
        vals.clear();
        vals.pushBack(value);
    }
    MinMaxFilter& operator=(const MinMaxFilter& input)
    {
        size = input.size;
        vals = input.vals;
        return *this;
    }
};
class WeightFilter
{
    size_t size;
    LinkedList<double> vals = {};
    double stdDev;

public:
    WeightFilter(size_t size, size_t stdDev) : size(size), stdDev(stdDev)
    {
    }
    WeightFilter(size_t size, size_t stdDev, double val) : WeightFilter(size, stdDev)
    {
        seed(val);
    }
    void update(double sensorVal)
    {
        vals.pushBack(sensorVal);
        while (vals.size() > size)
        {
            vals.popBase();
        }
    }
    double value()
    {
        // Get the mean
        double sum = 0;
        for (auto& val : vals)
        {
            sum += val;
        }
        double mean = sum / vals.size();
        // Get the standard deviation
        double stdev = sqrt(sum / vals.size());
        auto listCopy = vals;
        // Remove all values that are outside the standard deviation
        for (auto& val : listCopy)
        {
            if (abs(val - mean) > stdDev)
            {
                listCopy.popCurrent();
            }
        }
        // Get the mean of the remaining values
        sum = 0;
        for (auto& val : listCopy)
        {
            sum += val;
        }
        return sum / listCopy.size();
    }
    operator double()
    {
        return value();
    }
    void seed(double value)
    {
        vals.clear();
        vals.pushBack(value);
    }
};
#endif
