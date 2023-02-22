#include "Filters.h"

SMA::SMA(size_t size) {
    this->size = size;
}
SMA::SMA(size_t size, double val) : SMA(size) {
    seed(val);
}
void SMA::update(double sensorVal) {
    vals.pushBack(sensorVal);
    while (vals.size() > size) {
        vals.popBase();
    }
}
double SMA::value() {
    double sum = 0;
    for (auto& val : vals) {
        sum += val;
    }
    return sum / vals.size();
}
SMA::operator double() {
    return value();
}
void SMA::seed(double value) {
    vals.clear();
    vals.pushBack(value);
}
SMA& SMA::operator=(const SMA& input) {
    size = input.size;
    vals = input.vals;
    return *this;
}

WeightFilter::WeightFilter(size_t size, size_t stdDev) : size(size), stdDev(stdDev) {
}
WeightFilter::WeightFilter(size_t size, size_t stdDev, double val) : WeightFilter(size, stdDev) {
    seed(val);
}
void WeightFilter::update(double sensorVal) {
    vals.pushBack(sensorVal);
    while (vals.size() > size) {
        vals.popBase();
    }
}
double WeightFilter::value() {
    // Get the mean
    double sum = 0;
    for (auto& val : vals) {
        sum += val;
    }
    double mean = sum / vals.size();
    auto listCopy = vals;
    // Remove all values that are outside the standard deviation
    for (auto& val : listCopy) {
        if (abs(val - mean) > stdDev) {
            listCopy.popCurrent();
        }
    }
    // Get the mean of the remaining values
    sum = 0;
    for (auto& val : listCopy) {
        sum += val;
    }
    return sum / listCopy.size();
}
WeightFilter::operator double() {
    return value();
}
void WeightFilter::seed(double value) {
    vals.clear();
    vals.pushBack(value);
}
