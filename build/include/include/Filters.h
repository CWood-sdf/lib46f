#ifndef EMA_FILTER_H
#define EMA_FILTER_H
#include "stuff.h"
template <class G, class T>
G converter_def(T val);
template <class Get_T, class T = Get_T, Get_T (*converter)(T) = converter_def>
class BasicEMA
{
    double alpha;
    T val;
    T oldVal;

public:
    BasicEMA(double alpha);
    BasicEMA(double alpha, T val);
    void update(T sensorVal);
    T& valRef();
    Get_T value();
    operator Get_T();
    void seed(T value);
    BasicEMA<Get_T>& operator=(const BasicEMA<Get_T>& input);
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
    SMA(size_t size);
    SMA(size_t size, double val);
    void update(double sensorVal);
    double value();
    operator double();
    void seed(double value);
    SMA& operator=(const SMA& input);
};

class WeightFilter
{
    size_t size;
    LinkedList<double> vals = {};
    double stdDev;

public:
    WeightFilter(size_t size, size_t stdDev);
    WeightFilter(size_t size, size_t stdDev, double val);
    void update(double sensorVal);
    double value();
    operator double();
    void seed(double value);
};
#endif
