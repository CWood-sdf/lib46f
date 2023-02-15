#ifndef BEZIER_H
#define BEZIER_H
#include "EPA_Tracker.h"
#include <deque>
// Makes an array of Vectors that can be efficiently modified at both ends
struct VectorArr
{
    // Underlying storage type
    // Leave it public just because
    deque<PVector> arr = deque<PVector>();

    /**
     * @brief Construct a new Vector Arr object
     *
     * @param list A list of vectors to initialize the array with
     */
    VectorArr(std::initializer_list<PVector> list);
    VectorArr() = default;
    /**
     * @brief Construct a new Vector Arr object
     *
     * @param other The other array to copy
     */
    VectorArr(const VectorArr& other)
    {
        arr = other.arr;
    }
    VectorArr(VectorArr&&) = default;

    /**
     * @brief Get the begin iterator
     *
     * @return decltype(arr.begin())
     */
    decltype(arr.begin()) begin();
    /**
     * @brief Get the end iterator
     *
     * @return decltype(arr.end())
     */
    decltype(arr.end()) end();
    /**
     * @brief Get the vector at index i
     *
     * @param i the index
     * @return PVector& the vector at index i
     */
    PVector& operator[](int i);
    /**
     * @brief Remove the element at index i
     *
     * @param i the index
     */
    void pop(int i);
    /**
     * @brief Remove the first element
     *
     */
    void popBase();
    /**
     * @brief Remove the last element
     *
     */
    void popEnd();
    /**
     * @brief Get the size of the array
     *
     * @return size_t the size of the array
     */
    size_t size();
    /**
     * @brief Push a vector to the end of the array
     *
     * @param v the vector to push
     */
    void push(PVector v);
    /**
     * @brief Push a vector to the beginning of the array
     *
     * @param v the vector to push
     */
    void pushBase(PVector v);

    /**
     * @brief Return the last element
     *
     * @return PVector& A reference to the last element
     */
    PVector& last();
    /**
     * @brief Return the first element
     *
     * @return PVector& A reference to the first element
     */
    PVector& first();

    VectorArr& operator=(const VectorArr& v);
    VectorArr& operator=(VectorArr&& v);

    /**
     * @brief Gets the length of the curve formed by the points in the array
     *
     * @return double The length of the curve
     */
    double getCurveLength();
};

/**
 * @brief Creates a bezier curve
 *
 * @param ptArr The control point array
 * @param inc The increment to the t-value use
 * @return VectorArr The array of points on the curve
 */
VectorArr bezierCurve(VectorArr ptArr, double inc = 1.0 / 50.0);
/**
 * @brief Returns the derivative of a bezier curve at each point
 *
 * @param ptArr The control point array
 * @param inc The increment to the t-value use
 * @return VectorArr The array of derivative points
 */
VectorArr bezierDerivative(VectorArr ptArr, double inc = 1.0 / 50.0);
/**
 * @brief Returns the left and right normals at each curve point
 *
 * @param ptArr The control point array
 * @param dist The distance away from the curve to place the normals
 * @param inc The increment to the t-value use
 * @return pair<VectorArr, VectorArr> The left and right normals
 */
pair<VectorArr, VectorArr> bezierCurveNormalLR(VectorArr ptArr, double dist = 1.0, double inc = 1.0 / 50.0);
/**
 * @brief Returns the curvature of the curve at each point
 *
 * @param ptArr The control point array
 * @param inc The increment to the t-value use
 * @return vector<double> The curvature at each point
 */
vector<double> bezierCurvature(VectorArr ptArr, double inc = 1.0 / 50.0);
#endif
